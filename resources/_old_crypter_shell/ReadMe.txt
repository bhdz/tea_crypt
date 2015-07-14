For Dobry:
Kopele tova e stariat algoritum 
bez izmenoiasht se 256 bitov kluch 
NO s noviat shell!
pishi dir i vish kvo stava! :))

Author:
Boril Boyanov
You can reach me at borko_b@yahoo.com


[USAGE]
TEA Encrypting shell v2.0

Quick reference:

1. commands

1.1 code 

description: encodes files or directories

usage:
>code readme.txt 
	will encode a single file : redme.txt
	and will ask for password like this:

>Enter password: ****
	passwords are 16 chars max
switches:
	"-r" : recursive action :
	will encode every readme.txt in this path and
	its subdirectories
	example:
	"code *.zip -r" will encode every zip in 
	current path and sub-paths

	"|" : (after the file name)
	delimiter of files:
	example
	"code *.zip|*.jpg|readme.txt -r"
	will encode *zip and *.jpg and every readme.txt
	in current path and sub-paths

2.0 decode
	same as code but it decodes the files/paths ! :)

3.0 exit  Terminates execution

ANY other command is passed to the command prompt of 
	Windows so you'll have such nice things as:
	dir, cd,cd.., ren, copy,del,deltree
	and so on!: )


[DISCLAIMER]
This source comes with the GPL (General Public License)
license
So I do not take money for this as i don't take any
responsibility for any losses (financial or whatever!)
Use this prog at your risk!
This is Open Source Software So look below:
(You should contact me if you want to change the code)
(You should NEVER close this source)
(You should NEVER sell this code)
(You should change the author of this code (its me) )

Hashfunc2 is made by Berkely DB
CODE and DECODE are made by (oh! i don't know)
The algorithm of encrypting is called 
TEA (T)iny (E)crypting (A)lgorithm