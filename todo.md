## ft_tokenize
The naïve approach: use space symbols as delimiters while treating several successive space symbols as one delimiter

Improvement: treat quotes: everything between them is one token
 - upon encountering a quote, try to find the closing quote after it
   - if failed, then treat this quote as a regular character
   - if successful, then treat everything between them as one token
     - `'`: just remove the quotes
     - `"`: remove the quotes while expanding variables starting with `$`

<details>
<summary><tt>'</tt> bash-inputting</summary>
<p>

```
bash-3.2$ echo '      hi there' '     and this as well'
      hi there      and this as well
```
```
bash-3.2$ touch blah'   'and'   'this.txt
bash-3.2$ ls
blah   and   this.txt
```

</p>
</details>

<details>
<summary><tt>"</tt> bash-inputting</summary>
<p>

```
bash-3.2$ " blah $HOME hi" blah
bash:  blah /Users/amamian hi: No such file or directory
```

</p>
</details>

`ft_tokenize` shouldn't deal with boolean operators and parentheses, it should treat `(` as invalid input

<details>
<summary>Run these tests</summary>
<p>

```
minishell$ echo''two three
minishell$ echo"$HOME "two three
minishell$     "e"cho hello
```

</p>
</details>

## How to handle boolean operators and parentheses
`minishell`'s command execution logic has to be modified so that execution of commands is made recursively based on boolean operators present in `line` provided to `minishell`.

Pseudocode of how it should look like:
```
ft_exec
	find a boolean operator
	IF no boolean operators THEN
		status = execute the command
		return status
	status = ft_exec the expression to the left of the boolean operator
	IF operator is and AND status is fail
		return status
	status = ft_exec the expression to the right of the boolean operator
	return status
```
(note: this `ft_exec` has nothing to do with `ft_exec` in `ft_interpret.c`)

Treat expressions with more than two boolean operators as follows:

```
 A  &&  B  &&  C  &&  D
(A) && (B  &&  C  &&  D)
       (B) && (C  &&  D)
	          (C) && (D)
```

### Find a boolean operator
First try to find a boolean operator outside of parentheses by skipping expressions in parentheses: `paren_num++` whenever `(` is encoutered, `paren_num--` whenever `)` is encountered; stop as soon as `paren_num` is `0` and any boolean expression is encountered





F && ((D && E) || ((A || B) && C)) || (G && H)


<details>
<summary>Some shell behavior</summary>
<p>

```
bash-3.2$ echo $HOME
/Users/amamian
```
```
bash-3.2$ echo '$HOME'
$HOME
```
```
bash-3.2$ echo "$HOME"
/Users/amamian
```
```
bash-3.2$ echo $HOMEE
bash-3.2$
```
```
bash-3.2$ echo $HOMEE'andthen'$HOME
andthen/Users/amamian
```
```
bash-3.2$ echo $HOMEE 'andthen' $HOME
andthen /Users/amamian
```
```
bash-3.2$ echo $$
23298 [current pid]
```
```
bash-3.2$ echo $
$
```
```
bash-3.2$ echo $$$
23298$
```
```
bash-3.2$ echo $$$$
2329823298
```

valid chars that env var can
                             -consist of and can begin with: upper/lowercase letters, underscores
                             -consist of                   : numbers

Double expansions do not take place:
```
bash-3.2$ tar_tar='$tur'
bash-3.2$ echo $tar_tar
$tur
```

If heredoc is non-quoted, then env vars inside of it are ALWAYS expanded, no matter whether they themselves are quoted or not:
```
bash-3.2$ cat << END
> $HOME
> END
/Users/amamian
```
```
bash-3.2$ cat << END
> "$HOME"
> END
"/Users/amamian"
```
```
bash-3.2$ cat << END
> '$HOME'
> END
'/Users/amamian'
```
</p>
</details>


<details>
<summary>Irrelevant notes</summary>
<p>

## Skip invalid inputs
Count the number of quotes, if odd then print out "Invalid input" and wait for the next input

### How to count the number of quotes
```
bash-3.2$ echo 'blah "blah'"
> 
bash-3.2$ echo 'blah "blah'
blah "blah
bash-3.2$ echo "blah 'blah"
blah 'blah
bash-3.2$ echo "blah 'blah"'
> 
bash-3.2$ 'echo'""

```
Keep track of the number of quotes with `s_quotes` and `d_quotes`. Then, if `'` (`"`) is encountered, increment `s_quotes` (`d_quotes`) and stop paying attention to `"`s (`'`s) as long as another `'` (`"`) is not encountered. If, by the time `line` is over, if the value of either of `s_quotes` and `d_quotes` is odd, then the input is invalid.

</p>
</details>

<details>
<summary>Comments to ft_refineline</summary>
<p>

```
$HOME -> /Users/arman
'$HOME' -> $HOME
"$HOME" -> /Users/arman
$HOMEE -> (nothing)
$HOMEE'andthen'$HOME -> andthen/Users/arman
$HOMEE 'andthen' $HOME -> andthen /Users/arman
$$ -> current pid (e.g. 23298)
$ -> $
$$$ -> 23298$
$$$$ -> 2329823298

valid chars that env var can
                             -consist of and can begin with: upper/lowercase letters, underscores
                             -consist of                   : numbers

Double expansions do not take place:
bash-3.2$ tar_tar='$tur'
bash-3.2$ echo $tar_tar
$tur

If heredoc is non-quoted, then env vars inside of it are ALWAYS expanded, no matter whether they themselves are quoted or not:
bash-3.2$ cat << END
> $HOME
> END
/Users/arman
bash-3.2$ cat << END
> "$HOME"
> END
"/Users/arman"
bash-3.2$ cat << END
> '$HOME'
> END
'/Users/arman'
```

</p>
</details>

<details>
<summary>Comments to ft_tokenize</summary>
<p>

```
// anything in double parentheses is not executed and its return status is set to false (e.g. `((pwd)) && ls` doesn't print out anything)
//
// execution stops as soon as the global state is true or false (e.g. `pwd || ls` only executes `pwd`, `((pwd)) && ls` doesn't print out anything)
```
</p>
</details>
