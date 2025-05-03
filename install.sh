#!/bin/bash

# SimpleOS 安装脚本

# 检查是否在WSL环境中运行
if ! grep -q Microsoft /proc/version 2>/dev/null && ! uname -r | grep -qi microsoft; then
    echo "错误：检测到当前在Windows PowerShell环境中运行脚本"
    echo "SimpleOS安装脚本必须在WSL (Windows Subsystem for Linux) 环境中运行"
    echo ""
    echo "请按照以下步骤操作："
    echo "1. 确保已安装WSL，如未安装请访问：https://aka.ms/wslstore"
    echo "2. 在当前目录打开PowerShell，执行以下命令："
    echo "   wsl"
    echo "3. 进入WSL后，切换到项目目录："
    echo "   cd \"$(wslpath -u "$(pwd)")\"" 
    echo "4. 执行安装脚本："
    echo "   chmod +x install.sh && ./install.sh"
    echo ""
    echo "如果遇到问题，请确保："
    echo "- WSL已正确安装并启动"
    echo "- 当前用户具有执行WSL的权限"
    echo "- 项目文件可以被WSL环境访问"
    exit 1
fi

# 获取脚本所在目录的绝对路径并转换为WSL路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WSL_PATH="$SCRIPT_DIR"

# 如果路径包含重复的/mnt/，则修正它
if [[ "$WSL_PATH" =~ /mnt/[a-zA-Z]/mnt/[a-zA-Z]/ ]]; then
    WSL_PATH=$(echo "$WSL_PATH" | sed -E 's#/mnt/[a-zA-Z]/mnt/([a-zA-Z]/)#/mnt/\1#')
fi

# 检查当前目录是否为项目根目录
if [ ! -f "$WSL_PATH/install.sh" ] || [ ! -f "$WSL_PATH/Makefile.linux" ]; then
    echo "错误：未找到必要的项目文件"
    echo "当前WSL路径：$WSL_PATH"
    echo "提示："
    echo "1. 请确保您在SimpleOS项目根目录中运行此脚本"
    echo "2. 如果您在Windows中打开了WSL，请执行以下命令："
    echo "   cd /mnt/d/os  # 请根据实际项目位置调整路径"
    echo "3. 然后重新执行安装脚本："
    echo "   chmod +x install.sh && ./install.sh"
    exit 1
fi

cd "$WSL_PATH" || {
    echo "错误：无法切换到WSL路径：$WSL_PATH"
    exit 1
}

# 确保在正确的目录中运行
if [ ! -f "Makefile.linux" ]; then
    echo "错误：请确保在SimpleOS项目根目录中运行此脚本"
    echo "当前WSL路径：$WSL_PATH"
    echo "提示："
    echo "1. 确保已切换到正确的项目目录"
    echo "2. 检查Makefile.linux文件是否存在"
    echo "3. 如果在Windows中，请先使用'wsl'命令进入Linux环境"
    exit 1
fi

# 检查并更新软件源
echo "更新软件源列表..."

# 检查网络连接
ping -c 1 www.baidu.com > /dev/null 2>&1 || {
    echo "错误：网络连接不可用，请检查网络设置"
    exit 1
}

# 尝试使用阿里云镜像源
echo "配置阿里云镜像源..."
sudo cp /etc/apt/sources.list /etc/apt/sources.list.backup
sudo sed -i 's/deb.debian.org/mirrors.aliyun.com/g' /etc/apt/sources.list
sudo sed -i 's/security.debian.org/mirrors.aliyun.com/g' /etc/apt/sources.list
sudo sed -i 's/ftp.debian.org/mirrors.aliyun.com/g' /etc/apt/sources.list

# 更新软件源
sudo apt-get update || {
    echo "错误：软件源更新失败"
    echo "正在还原原始软件源配置..."
    sudo cp /etc/apt/sources.list.backup /etc/apt/sources.list
    echo "请检查以下问题："
    echo "1. 确保系统时间正确"
    echo "2. 检查DNS设置"
    echo "3. 尝试手动编辑 /etc/apt/sources.list"
    exit 1
}

# 检查必要的构建工具
if ! command -v make >/dev/null || ! command -v gcc >/dev/null; then
    echo "正在安装必要的构建工具..."
    sudo apt-get install -y build-essential || {
        echo "错误：构建工具安装失败"
        echo "请尝试以下操作："
        echo "1. 检查网络连接"
        echo "2. 确保系统时间正确"
        echo "3. 手动运行：sudo apt-get install -y build-essential"
        exit 1
    }
fi

# 构建Debian包
build_deb_package() {
    echo "开始构建Debian包..."
    # 设置安装目录
    DESTDIR="$(pwd)/debian/simpleos"
    echo "设置安装目录：$DESTDIR"

    # 清理旧的构建目录
    rm -rf "$DESTDIR"

    # 创建必要的目录
    echo "创建系统目录..."
    mkdir -p "$DESTDIR/usr/local/bin"
    mkdir -p "$DESTDIR/usr/local/lib/simpleos"
    mkdir -p "$DESTDIR/usr/local/share/doc/simpleos"
    mkdir -p "$DESTDIR/etc/ld.so.conf.d"
    mkdir -p "$DESTDIR/DEBIAN"

    # 编译源代码
    echo "编译SimpleOS..."
    make -f Makefile.linux clean
    make -f Makefile.linux

    # 安装文件
    echo "安装SimpleOS..."
    make -f Makefile.linux install "DESTDIR=$DESTDIR"

    # 复制DEBIAN控制文件
    # 避免将目录复制到自身
    cp -r DEBIAN/control "$DESTDIR/DEBIAN/"
    # 复制其他必要的DEBIAN文件（如果存在）
    if [ -f DEBIAN/postinst ]; then
        cp DEBIAN/postinst "$DESTDIR/DEBIAN/"
        chmod 755 "$DESTDIR/DEBIAN/postinst"
    fi
    if [ -f DEBIAN/prerm ]; then
        cp DEBIAN/prerm "$DESTDIR/DEBIAN/"
        chmod 755 "$DESTDIR/DEBIAN/prerm"
    fi

    # 构建Debian包
    echo "打包SimpleOS..."
    dpkg-deb --build "$DESTDIR" .

    echo "SimpleOS Debian包构建完成！"
    echo "生成的包文件：simpleos_1.0.0_amd64.deb"
}

# 本地安装
local_install() {
    echo "开始本地安装..."
    # 创建设备节点（如果需要）
    if [ ! -e /dev/fb0 ]; then
        sudo mknod /dev/fb0 c 29 0
        sudo chmod 666 /dev/fb0
    fi

    # 安装依赖
    sudo apt-get install -y libc6

    # 安装Debian包
    if [ -f simpleos_1.0.0_amd64.deb ]; then
        sudo dpkg -i simpleos_1.0.0_amd64.deb
        echo "SimpleOS安装完成！"
        echo "请运行 'simpleos' 启动系统"
    else
        echo "错误：未找到SimpleOS安装包"
        exit 1
    fi
}

# 执行构建和安装
build_deb_package
local_install