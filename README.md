make
sudo insmod phil.ko
lsmod | grep phil  // to check whether module is loaded or not.
dmesg
sudo rmmod phil
