---
layout: post
title: "Move — a programming language"
disqus_comments: true
---

As I'm very interested in human-computer interaction, programming languages and the act of looking at things from a wider perspective, I decided to design a programming language. First, I scribbled down some key points:

- This language should be suitable as a "first programming language" for people not previously familiar with programming computers.

- As low barrier as possible for running it — i.e. it should run on as many platforms as possible needing as few additional tools as possible.

- Have a simple and limited set of "words" (language constructs), hopefully aiding the processes of learning how to write programs.

So I spent a full Sunday two weeks ago punching out a prototype implementation which I was so happy with that I've decided to continue working with this idea and today I finally wrapped everything together with documentation, a guide and throughly tested code.

I call it [Move](http://movelang.org/). Here is a simple program which print "Hello John" three times:

    hello = ^(name){ "Hello "+name }
    repeat {times: 3} ^{
      print hello {name: "John"}
    }

The language is now stable and will probably not change (except for bugs and runtime library expansion) in the near future. JavaScript is normally my "universal", "go-to" language for everything from crunching log files and doing simple math to writing small networked programs and web sites. However, during these two weeks I've used Move instead and it just feels right: I get done quicker, less boiler plate code, easier to read, etc.

[![movelangdotorg](http://farm6.static.flickr.com/5214/5492500401_54b711ac5e_o.png)](http://movelang.org/)

Move *compiles down to optimized JavaScript* and can run on any ES3 or higher speced JS platform (like a web browser or Node.js). Move also contains a runtime library which not only provides a few carefully crafted functions but also *normalizes the JS environment to the ES5 standard*, relieving you from feature-testing for "Object.create" or doing work-arounds for things like "Array.isArray". Move is open source under a permissive MIT license and available at [https://github.com/rsms/move](https://github.com/rsms/move).

## Differences to JavaScript

As mentioned, Move compiles to JavaScript and runs in any ES3 or more modern JavaScript runtime, so comparing Move to JavaScript comes naturally. The first version of Move was actually a superset of JavaScript, allowing pristine JavaScript inside Move. As the language evolved and was simplified, some changes where made (for instance: "&#x5e;" always meaning "function" and never meaning "xor", introducing the "xor" operator).

Here's a summary of the differences between Move and JavaScript:

- **Functions** are declared using one single expression: `^(arg1){...}` and can be called using either **keyword arguments** or positional arguments. Functions without arguments can be written as `^{...` or `^(){...` (parenthesis are optional).
  
  - In JavaScript, there are two different ways to define a function: using the function expression and the function declaration statement, the latter having subtle restrictions. Move only has function expressions and all but strictly anonymous functions are named, providing a richer stack trace when debugging.
  
  - As functions, or lambdas, are the key awesomeness of JavaScript and is a very light-weight thing, the "function" keyword and boilerplate argument parens just need to be easier to write. Move uses the "&#x5e;" keyword and does not enforce `()` for argument-less functions.

  - The last statement in a function is automatically returned (the "product" of calling a function).

  - Functions can be called using the `foo {key: value}` syntax, passing arguments by name. This greatly increases code readability and eliminates the tedious `foo(null, null, null, null, true)` style when only one of many arguments are relevant.

  - Functions can be called using the `foo value, ...` syntax, passing arguments in a simple succession. This feature allows Move to have a very limited set of language keywords and instead implement things like print, repeat, create, etc as regular functions. For instance, this allows for creating DSL-like extensions to the language.

  - When defining functions, any argument can be given a default value using either `:` or `=`. E.g. `foo = ^(bar, baz: 4, names=[], age) {...`.

- **Variables** are implicitly declared -- Move will automatically declare (i.e. "create and attach") a newfound variable in the scope which it first was used. The use of "var" produces a syntax error. A variable will only be declared if there's no variable with the same name which is reachable from the current scope (this is possible since the Move compiler is AST-aware).

- **No commas** required to terminate expressions. Move will determine when a comma is needed so you don't have to (and the code gets more readable).

- No type-coercing **equality operators** -- in Move, `==` does not perform type coercion and behaves just like `===` in JavaScript. Same goes for `!=`.

- **@-shorthand** for accessing properties of the `this` object. E.g. `this.foo = 5` can be written as `@foo = 5`.

- **Importing** CommonJS modules can be done using the `import` statement. E.g. rather than the tedious `var module1 = require("module1")` boilerplate, you can write `import module1`

- **Exporting** CommonJS modules is aided by the `export` statement. E.g. instead of writing `var foo = exports.foo = function () {...` you can write `export foo = ^{...` (or even `export foo` if `foo` is defined elsewhere).

- A small runtime library makes Move **ES5 normalized** even in non-ES5 environments. This means less feature probing and less boiler-plate code. Things like `Array.isArray`, `Object.create` and `String.prototype.trim` are all guaranteed to be available and function according to the ES5 standard.

Move is **designed for humans** which is the reason for why things like variable declarations and statement terminators has been removed -- they are simply not part of the essentials or essence of what a programmer should be thinking about. The computer is, or at least should be, better at deciding those things.


## Function the ultimate

The biggest difference to most other programming languages is how functions are created, handled and invoked. In Move (and JavaScript) functions are light-weight, first class objects which *should be used*. Writing `name = function name(x, y, z) {...` quickly becomes a tedious task when whipping up code, so Move simplifies the function expression syntax to `name = ^(x, y, z) {...`.

An example:

    delayed_print = ^(message, delay=1000) {
      after {delay: delay} ^{ print message }
    }
    delayed_print "Hello"                // "Hello" appears 1 second later

Calling functions with keyword arguments (or "named" arguments if you will) greatly increases readability and thus maintainability of code.

    delayed_print { message: "Hello", delay: 5000 }
    // "Hello" appears 5 seconds later

Move allows calling function without neither parenthesis nor curly brackets, enabling the use of regular functions to build DSL-like mini-languages. For instance, this program defines a "repeat" function which is then called to invoke an anonymous function three times, producing three "Hello":

    repeat = ^(times){ ^(block){
      if (times != undefined)
        for (i=0; i<times; ++i) if (block(i) == true) break
      else
        while (true) if (!block()) break
    }}
    
    repeat {times: 3} ^{
      print "Hello"
    }

This works since the "repeat" function produces an anonymous function (which when called invokes another function *times* number of times) which will be called due the short-hand call style in Move. The invocation above roughly compiles down to the following JavaScript:

    repeat({times: 3})(function () {
      print("Hello");
    });

There are other neat effects of the short-hand call style. For instance, consider the following Move code:

    print(JSON.stringify("1,2,3,4,5,1,4".split(",")))

Which can also be written like this:

    print JSON "1,2,3,4,5,1,4".split ","

Move comes with a few but carefully crafted built-in functions (which in true Move style can be manipulated or replaced at will):

- [`create`](http://movelang.org/#create) — create new objects based on other objects (object-oriented programming using differential inheritance).
- [`extend`](http://movelang.org/#extend) — extend an existing object by shallow-copying properties from another object.
- [`print`](http://movelang.org/#print) — visualize any value.
- [`repeat`](http://movelang.org/#repeat) — repeat something (a friendlier approach to "for (i = 0; i < ..." and "while (...".
- [`after`](http://movelang.org/#after) — perform something after a certain point in time.

[The "create" function](http://movelang.org/#create) is used to create new objects semantically based on other objects, achieving a very straight-forward approach to object-oriented programming:

    animal = { type: "an animal",
               toString: ^{ "I'm " + @type } }
    cat = create animal, { type: "furry" }
    print "Cat: " + cat  // --> "Cat: I'm furry"

In the above example, "cat" inherits from "animal" — adding, changing or removing properties on "animal" has an effect on what happens when accessing properties of the "cat" object.

Read more about [Prototype-based programming](http://en.wikipedia.org/wiki/Prototype-based_programming) and  [Differential inheritance on Wikipedia...](http://en.wikipedia.org/wiki/Differential_inheritance)


## Try to Move

I recommend checking out the [website at movelang.org](http://movelang.org/) (albeit being a work in progress) as well as the interactive console, perfect for playing around with Move, at [movelang.org/try](http://movelang.org/try/).

For TextMate junkies, there's also a Move bundle with syntax highlighting, commands for running programs, displaying compiled JavaScript code, AST viewer, etc. [Grab a snapshot of @master](https://github.com/rsms/move/zipball/master) and find Move.tmbundle in the "extras" directory.

### Installing Move for Node.js using NPM

First, you need to have [Node.js](http://nodejs.org/) and [NPM](http://npmjs.org/) installed, then in a terminal:

    $ npm install move

You should now have both the Move library (compiler and runtime) installed as well as a "move" program which can execute Move scripts.

    $ move my-move-program.mv

Or in a Node.js JavaScript file by at one point in time requiring the "move" module (which will enable loading of Move code in Node.js):

    require('move');
    require('./my-move-program');

Note that you can not embed Move into JavaScript but must separate the code into .mv files.
