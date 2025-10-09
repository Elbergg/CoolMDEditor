# CoolMD

CoolMD is a Markdown Engine written entirely in C (the tests use c++ though).

It currently implements

- headers

- paragraphs

- blockquotes

- bold

- italic

With more coming in the future!

## Usage:

```c
#include "coolmd.h"

char* text = "# hello";
char* result = compile_to_html(text);
```

## FAQ:

- Q: Is it the fastest engine out there?
  
  A: Probably not

- Q: Is it 100% [commonmark](https://commonmark.org/) compatible?
  
  A: Certainly not, however it tries to be with the features it implements

- Q: Is it ___COOL___?
  
  A: YES!

## Shoutouts:

Thanks to [babelmark](https://babelmark.github.io/) for being a great tool for this project

Also make sure to check out my [editor](https://github.com/Elbergg/CoolMDEditor) which uses this engine to renders its markdown, however it is still very early in developement.


