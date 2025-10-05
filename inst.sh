./run.sh
mkdir ~/.local/bin
cp build/gentle ~/.local/bin
echo "export PATH=\"$PATH:~/.local/bin\"" >> ~/.bashrc
source ~/.bashrc
