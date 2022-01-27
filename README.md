# yawc-cli
Yet Another Wordle Clone

A simple cli version of wordle with some support for custom word lists.

This was made as a short project to brush up on my C memory management. As nice
as memory managed languages are, you never know when you're gonna get thrust into
some low level/embedded stuff where you can't take data structures for granted.
Eventually, the plan is to add custom styles alongside word lists, but who knows
if I'll end up having time for that. I will likely at least have a command line
option to specify the word list being used rather than only work

## To Build

I used CMake to build this, but honestly, it's kind of overkill since the only
dependency is the math library, and I made no effort to ensure compatibility with
Windows. Honestly, it would probably work with windows but who knows. It would,
at the very least, work under WSL.

To build, I would make a build directory so the src directory doesn't get cluttered.
Here I will use `build`.

```bash
mkdir build
cd build
cmake ../src
make
```

## To Install/Run
This will generate a file called `yawc`, which you can move to somewhere in your path
or run from the build directory. However, you will need to set 2 environment vars, one
to the path where you will put all the word lists: `$YAWC_LISTS` Relative to the project
repo, this is `./lists`, but I would advise moving them to your `.config` directory 
under `yawc`. The second is the word list you would like to use: `$YAWC_DEFAULT`. This
is just the name of the directory under the list directory containing your word list. So,
for the included list, use `wordle`. You can add these exports to your `.bashrc` or `.profile`
for convenience.

## Word list spec
A word list consists of 2 files, a list of valid guesses, and a subset of words you would like to
be potential answers. These can be the same, but the answers file is not added to the list of
valid guesses, so make sure all words in answers are also in guesses. The answers and guesses
will be named `answers.txt` and `guesses.txt` respectively. The first line of each list should
be the length of each word. Each word should be all caps and should be the same length. There should
be no white space other than newlines after each word. The last character should be a newline.