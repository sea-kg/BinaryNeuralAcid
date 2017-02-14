# Reversehash

Experiment for reversing md5-hash function
Author by sea-kg


## Requirements

	$ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev

## Build 

	$ qmake reversehashd.pro
	
## Configure (all comands from root of the this repository)

	$ sudo ln -s `pwd`/var/www/html/reversehashd /var/www/html/reversehashd
	$ sudo ln -s `pwd`/usr/share/reversehashd /usr/share/reversehashd
	$ sudo ln -s `pwd`/reversehashd /usr/bin/reversehashd
	$ sudo ln -s `pwd`/etc/init.d/reversehashd /etc/init.d/reversehashd
	$ sudo ln -s `pwd`/etc/reversehashd /etc/reversehashd
	$ cp etc/reversehashd/conf.ini.example etc/reversehashd/conf.ini
	$ sudo update-rc.d -f reversehashd remove
	$ sudo update-rc.d reversehashd defaults
	$ nano /etc/reversehashd/conf.ini


## Manage

	http://localhost/reversehashd/manage/
	
## Try reverse 

	http://localhost/reversehashd/
