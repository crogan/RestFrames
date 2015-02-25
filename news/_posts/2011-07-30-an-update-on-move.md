---
title: An update on the programming language Move
layout: post
tags: move, programming
fb_comments: 25
---

<img src="http://movelang.org/res/logo.png" align="right" width="256">A [few months back](http://rsms.me/2011/03/02/move-programming-language.html) I wrote a programming language called [Move](http://movelang.org/). Before the advent of Move, JavaScript (on Node.js) was my universal language of choice. Two years earlier it was Python. During the last four months I have basically been exclusive with Move -- quick hacks, data mangling scripts, network services, websites, iphone apps... you name it. What initially was a fun week of language research and interviews with people, turned into a very usable programming language and library.

Today Move has evolved -- from what was first released on March 2, 2011 -- over nine releases, making the language even simpler and listening to user feedback.


## The web is the future

It's inevitable. The web (well, stuff over HTTP viewed in a web browser) is the app platform of tomorrow. Move provides a *normalized environment*, a uniform library with a standard CommonJS module system. This is actually a huge deal since what traditionally incurred the feeling that JavaScript was a second-class programming environment, should be hugely contributed to the vast number of bugs,  API differences and library discrepancies implied by the web browser landscape. Although the API of today's web browsers are generally coherent, there are still a bunch of differences your everyday JavaScript web browser engineer need to -- painfully -- be aware of and feature-test for.

For example, a common thing to do is to iterate over items in a list:

    someArray.forEach ^(item, i) {
      print i+':', item
    }
    someObject.forEach ^(key, value) {
      print key+':', value
    }

On modern browsers, that code will be very fast as it's implemented natively.

In JavaScript, you would need to do some feature testing...

    var i, key, apply = function (item, i) {
      console.log(i+':', item);
    };
    if (typeof Array.prototype.forEach === 'function') {
      someArray.forEach(apply);
    } else {
      for (i = 0; i < someArray.length; ++i)
        apply.call(someArray, someArray[i], i);
    }
    var HOP;
    if (typeof Object.prototype.hasOwnProperty === 'function') {
      HOP = Object.prototype.hasOwnProperty;
    } else {
      HOP = function (name) { return this[name] !== undefined; };
    }
    apply = function (key, value) {
      console.log(key+':', value);
    };
    for (key in someObject) {
      if (HOP.call(someObject, key))
        apply.call(someObject, key, someObject[value]);
    }

I would chose the Move way any day of the week.

"But I'll just use jQuery" you say and wonder if this normalization thingy in Move is just yet another library. No. Move provides a [ES5](http://www.ecma-international.org/publications/standards/Ecma-262.htm) environment, equivalent to a modern version of Chrome or Safari. As time progress, the ES5 standard will be completely implemented on all platforms. When that happens you no longer need a 3rd party library (theoretically speaking). You're *investing* in learning the future standard instead of the API of a 3rd party compatibility library.

Note that some features of ES5 requires compiler, vm or otherwise host-level support in older browsers, which is thus impossible to "glue together". On the topic, [Annotated ECMAScript 5](http://es5.github.com/) is a great and accessible documentation of the ES5 standard.

Another pretty awesome feature of Move is that you get a standard CommonJS module system. That basically means that slicing up your code into modules is easy peacy:

    import foo, bar
    x = foo.someFunction()
    y = bar x
    export z = y * x

Part of the Move language are the two keywords *import* and *export*. *Import* is a convenience preprocessor for the CommonJS *require* function.

    import foo
    
Compiles to:

    foo = require 'foo'

The *export* keyword similarly converts a statement like:

    export foo = 5

To:

    exports.foo = foo = 5

The `exports` variable is part of the CommonJS module specification and represents the API that your module (source file) provides (exports) to other source code.

It's completely optional to use these convenience keywords.

Here's a more complete example of modules in a web browser environment:

    <script src="http://movelang.org/move.js"></script>
    <script type="text/move" module="bar">
    import foo, capitalize
    export sayHello = ^(name) {
      print foo.makeHello capitalize name
    }
    </script>
    <script type="text/move" src="capitalize.mv"></script>
    <script type="text/move" module="foo">
    export makeHello = ^(name) { 'Hello '+name+'!' }
    </script>
    <script type="text/move">
    import bar
    bar.sayHello 'worlds'
    </script>


## Slicing and dicing collections

Move falls into the category of "end-user data" programming languages (I just made that up), thus dealing with text and lists of items is a very common task. Move provides a slice syntax which should come natural to e.g. Python programmers:

    print "hello"[1:3]  # "el"
    
    x = [1,2,3,4]
    print x[1:3]        # [2, 3]
    x[1:3] = 9
    print x             # [1, 9, 4]
    x[1:] = [9, 10]
    print x             # [1, 9, 10]

Since Move is heavily based around the core concept of first-class functions, this slice syntax compiles to simple function calls:

    foo[1:3]
    foo[1:3] = 9
    foo[1:] = 9

Which is equivalent to:

    foo.slice 1, 3
    foo._move_setSlice 1, 3, 9
    foo._move_setSlice 1, undefined, 9

This means that *any object* can support slices by simply implementing both or one of `slice(startIndex, endIndex) → list` (getter) and `_move_setSlice(startIndex, endIndex, value) → list`.


## Embedded HTML and compiler preprocessor API

This is a pretty awesome feature: HTML literals.

    url = "http://movelang.org/res/logo.png"
    img = <img src="{url}"/>
    img.width = 500
    document.body.appendChild img

With this feature comes the ability to plug in preprocessors to the Move compiler. Embedded HTML (or EHTML for short) is currently the only plugin that ships with Move, but the preprocessor API is pretty simple: Create a module which exports a *process* function `process(string moveSource, object compilerOptions) → string moveSource`:

    export process = ^(source, options) {
      # Transform source
      source
    }

Then the preprocessor need to be registered with the compiler:

    move.preprocessors['my-preprocessor'] = process

Finally, specifying the preprocessor when compiling:

    move.compile {source:source, preprocess:['ehtml', 'my-preprocessor']}

The order of which preprocessors are specified in the "preprocess" argument to `move.compile` decides which is applied first. By default Move will enable the "ehtml" (Embedded HTML) preprocessor by default when run in a en environment that provides a DOM (i.e. a web browser).


## Classes -- Object factories with prototype chains

Since the birth of Move "class definition" has been a common feature request: Ability to define prefab prototype chains.

Move leans toward the "good parts of JavaScript", thus the "new" keyword (added to JavaScript simply to make it look like Java -- oh politics) should be avoided. `Object.create` is the recommended way of creating new objects based on custom prototypes. Still, `Object.create` is limited to creation (which is a good thing). From the eyes of `Object.create`, there's no notion of constructor, or rather; there's no difference between a constructor function or any other function.

As [Douglas Crockford states](http://yuiblog.com/blog/2006/11/13/javascript-we-hardly-new-ya/) it:

> JavaScript is a prototypal language, but it has a new operator that tries to make it look sort of like a classical language. That tends to confuse programmers, leading to some problematic programming patterns.

Avoid the "new" keyword and use literals or factory functions instead.

Say hello to Move's *class* construction function:

    Animal = class {
      age: 1,
      toString: ^{ "I'm a "+@kind }
    }

    elephant = Animal {kind:"slow and kind fella"}
    print Text elephant  # "I'm a slow and kind fella"
    print elephant.age   # 1

In the above case we define the factory *Animal*, having a prototype with two values: *age* and *toString*. The *Animal* factory produce objects with a prototype of *Animal.prototype*.

We can create another factory which prototype inherit from the *Animal* prototype:

    Cat = class Animal, {
      constructor: ^(name, age) {
        @kind = "furry little creature"
        name && (@name = name)
        age && (@age = age)
      },
      toString: ^{
        s = Animal.prototype.toString.call this
        s + " named " + @name
      }
    }

    cat = Cat {name:"Busta", age:10}
    print Text cat  # "I'm a furry little creature named Busta"
    print cat.age   # 10

Note that we defined a *constructor* function on the prototype. In this case, calls to the factory will invoke that function (instead of the implicit and generic *create* function).

Constructor functions (or you could think of them as initialization functions) need not be defined on subclasses in order to invoke a superclass's constructor:

    Zelda = class Cat, {
      name: "Zelda",
      toString: ^{ "I'm awesome and my name is "+@name }
    }

Since the above *Zelda* prototype does not define a constructor, the parent prototype's constructor will be called (that is, *Cat.prototype.constructor*) when the *Zelda* factory is invoked

    zelda = Zelda {age:5}
    print Text zelda  # "I'm awesome and my name is Zelda"
    print zelda.age   # 5

As usual with Move, `class` is simply a runtime function (`__move.runtime.__class`).

Actually, the *Zelda* factory and prototype chain can be described (and traversed) like this:

    Zelda                                # [function]
    Zelda.prototype                      # { name:"Zelda", toString:[function] }
    Zelda.prototype.prototype            # -> Cat.prototype
                Cat.prototype            # { constructor:[function], toString:[function] }
                Cat.prototype.prototype  # -> Animal.prototype
                       Animal.prototype  # { age:1, toString:[function] }
                       Animal.prototype.prototype  # Object.prototype



## Helpful command line interface

The `move` CLI tool (a program with a text interface) acts both as an operating system entry-point for Move programs run directly on a system and as a utility for dealing with and processing Move code.

Run a Move program:

    move foo.mv
    move run foo.mv

Run Move code from *stdin*:

    echo 'print "hello"' | move run
    move run < foo.mv

Output the parser's Abstract Syntax Tree that represents your code, as JSON:

    move compile --ast foo.mv

Show the JavaScript generated by the compiler:

    move compile foo.mv

Show the list of global options and available commands:

    move -h
    move --help

Show documentation for a specific command ("compile" in this example):

    move help compile

Create a stand-alone web-browser compatible JavaScript file from one or more source files:

    move compile --bundle-standalone foo.mv bar.mv leet.js
    move compile --bundle-standalone --basedir lib lib/*.{mv,js}
    move compile --bundle-standalone --basedir lib --output bundle.js lib/*.{mv,js}

This article and examples in it assumes the latest release of Move at the time of publishing this (0.4.2).

More information on Move can be found at [movelang.org](http://movelang.org/)







