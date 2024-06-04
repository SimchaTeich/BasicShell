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
Because myshell supports the navigation of the last commands by the arrows, without the need to press enter to receive the arrow, **so in this version it is not possible to edit incoming commands.**

In other words, you should be precise in entering each input.

## Examples
