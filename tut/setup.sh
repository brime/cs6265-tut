#!/bin/bash

apt-get -y update

apt-get -y install gdb emacs vim git zsh gdb-multiarch python2.7 python-pip \
        python-dev libssl-dev libffi-dev build-essential execstack \
        rdiff-backup python3-pip gdbserver libelf-dev \
        linux-headers-$(uname -r)

# Install 32 bit libs
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get -y install libc6:i386 libncurses5:i386 libstdc++6:i386
sudo apt-get -y install libc6-dev-i386

# Enable ptrace
echo "kernel.yama.ptrace_scope=0" | sudo tee -a /etc/sysctl.conf
# reload sysctl
sysctl --system

# Installing tools
pip install -U setuptools
pip install pwntools
pip uninstall -y pyelftools
pip install pyelftools==0.24
pip install ROPGadget
pip install ropper
pip3 install pycrypto

timedatectl set-timezone 'EST'

sudo apt-get install sshpass
# sudo mv /home/vagrant/seclab /bin/seclab
# sudo chmod +x /bin/seclab

# pwndbg
cd /home/vagrant
git clone -b ubuntu18.04-final https://github.com/pwndbg/pwndbg
cd pwndbg
./setup.sh
echo "source $PWD/gdbinit.py" > /home/vagrant/.gdbinit

# Installing kflag module
cd /home/vagrant/kflag
sudo make clean
sudo make all
sudo make insmod
mv ./seclab /etc/init.d/seclab
chmod +x /etc/init.d/seclab
chown root:root /etc/init.d/seclab
ln -f -s /etc/init.d/seclab /etc/rc2.d/S50seclab
systemctl daemon-reload
service seclab start

# deploying tutorials
sudo rsync -a /home/vagrant/tuts/ /home/
sudo chown -R vagrant: /home/lab*
sudo cp -f /home/vagrant/seclab /bin/

