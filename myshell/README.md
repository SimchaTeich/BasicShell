# MyShell
First homework in an advanced programming course.

## How to use
```
make
```
```
./myshell
```

## Functionality

We were asked to write our own shell. This shell is just a mini version of the original shell.<br />

MyShell supports the following:<br />
* `<command> &`$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$ - run the `<commmand>` at the backgrount
* `<command> > file`$~~~~~~~~~~~~~~~~~~~~~~~~~$- replace the original **stdout** with `file`
* `<command> 2> file`$~~~~~~~~~~~~~~~~~~~~~~~$- replace the original **stderr** with `file`
* `<command> >> file`$~~~~~~~~~~~~~~~~~~~~~~~$- like `>` but **append** to the file
* `<command> < file`$~~~~~~~~~~~~~~~~~~~~~~~~~$- replace the **stdin** with existing `file`
* `prompt = <string>`$~~~~~~~~~~~~~~~~~~~~~~~$- replace the current prompt sign with `<string>`
* `$<var> = <val>`$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$- create new environment variable
* `export $<var>` or `export <var>`$~$- export the environment variable var (make it gloabl)
* `read <var>`$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$- insets to the environment variable `<var>` new value from user
* `set`$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$- display all the environment variables 
* `<command> $<var>`$~~~~~~~~~~~~~~~~~~~~~~~~~$- replace `$<var>` with the correct value before run the `<command>`
* `<command> $?`$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$- replace `$?` with last command status before run the `<command>`
* `cd <dir>`$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$- change the current directory with `<dir>`
* `!!`$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$- run the last command again
* `history`$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$- displays a list of up to 20 recent commands
* `quit`$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$- quit the shell

* `<cmd> | <cmd> | ... | <cmd>`$~~~$- you can make as many pipes as you want

Also, myshell supports flow control:
```shell
if <command>
then
<command>
...
<command>
else      # optional
<command>
...
<command>
fi
```

In addition, by clicking on the <kbd>&uarr;</kbd> and <kbd>&darr;</kbd> arrows, you can navigate through the last 20 commands.

## Notes
In this simple shell the `&`, `>`, `>>`, `2>` and `<` commands only work at the end of a command (or in the last command in a chain of commands with Pipes) and only one of them can be selected at a time. 

For example, `ls -l > file` or `ls -l &` but not both `&` and `>`

## Examples
```
myshell$ date > myfile
myshell$ date -u >> myfile
myshell$ cat myfile
Tue 04 Jun 2024 07:10:31 AM EDT
Tue 04 Jun 2024 11:10:50 AM UTC
myshell$ wc -l < myfile
2
myshell$ prompt = ~~~~~~~$ 
~~~~~~~$ mkdir newdir
~~~~~~~$ cd newdir
~~~~~~~$ pwd
/home/seed/Desktop/BasicShell/myshell/newdir
~~~~~~~$ touch file1 file2 file3
~~~~~~~$ ls
file1  file2  file3
~~~~~~~$ !!
ls
file1  file2  file3
~~~~~~~$ echo $?
0
~~~~~~~$ ls nofile
ls: cannot access 'nofile': No such file or directory
~~~~~~~$ echo $?
512
~~~~~~~$ ls nofile 2> saveerror
~~~~~~~$ cat saveerror
ls: cannot access 'nofile': No such file or directory
~~~~~~~$ cat > colors.txt
blue
black
red
red
green
blue
green
red
red
blue
~~~~~~~$ cat colors.txt
blue
black
red
red
green
blue
green
red
red
blue
~~~~~~~$ cat colors.txt | cat | cat | cat
blue
black
red
red
green
blue
green
red
red
blue
~~~~~~~$ sort colors.txt | uniq -c | sort -r | head -3
      4 red
      3 blue
      2 green 
~~~~~~~$ history
   1 date > myfile
   2 date -u >> myfile
   3 cat myfile
   4 wc -l < myfile
   5 prompt = ~~~~~~~$
   6 mkdir newdir
   7 cd newdir
   8 pwd
   9 touch file1 file2 file3
  10 ls
  11 echo $?
  12 ls nofile
  13 echo $?
  14 ls nofile 2> saveerror
  15 cat saveerror
  16 cat > colors.txt
  17 cat colors.txt
  18 cat colors.txt | cat | cat | cat
  19 sort colors.txt | uniq -c | sort -r | head -3
  20 history
~~~~~~~$ if date | grep Fri
~~~~~~~$ then
~~~~~~~$ echo "Shabat Shalom"
~~~~~~~$ else
~~~~~~~$ echo "Hard way to go"
"Hard way to go"
~~~~~~~$ fi
~~~~~~~$ read msg
Bye Bye!
~~~~~~~$ $myname = annonymous
~~~~~~~$ echo $myname say $msg
annonymous say Bye Bye!
~~~~~~~$ quit
```
