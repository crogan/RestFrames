---
title: "Introducing RestFrames"
layout: post
tags: tutorial
photo_url: "/res/RestFramesTree.gif"
description: "Welcome to the RestFrames software library"
---

<img src="/res/RestFramesTree.gif" align="right" width="256"> Welcome to RestFrames. sdfadsf 
Here is some code *in the text*:

{% highlight C++ linenos %}
for(int i = 0; i < N; i++){
	int j = i;
}
// ...
MyClass* bla = new MyClass();
{% endhighlight %}

```C++ linenos
for(int i = 0; i < N; i++){
	int j = i;
}
// ...
MyClass* bla = new MyClass();

```


$$ \sum_{i} \phi(x,y) = \phi \left(  x_i, \right)  $$

$$ \left( \begin{array}{ccc}
      \phi(e_1, e_1) & \cdots & \phi(e_1, e_n) \\
      \vdots & \ddots & \vdots \\
      \phi(e_n, e_1) & \cdots & \phi(e_n, e_n)
    \end{array} \right) $$

$$ \left( \begin{array}{c}
      y_1 \\
      \vdots \\
      y_n
    \end{array} \right)$$


[Gotalk](https://github.com/rsms/gotalk) exists to make it easy for programs to *talk with one another over the internet*, like a web app coordinating with a web server, or a bunch of programs dividing work amongst eachother.

![A terribly boring amateur comic strip](/res/gotalk-comic.png)

Gotalk takes the natural approach of *bidirectional* and *concurrent* communication — any peer have the ability to expose "operations" as well as asking other peers to perform operations. The traditional restrictions of who can request and who can respond usually associated with a client-server model is nowhere to be found in gotalk.

## Gotalk in a nutshell

#### Bidirectional

There's no discrimination on capabilities depending on who connected or who accepted. Both "servers" and "clients" can expose operations as well as send requests to the other side.

#### Concurrent

Requests, results, and notifications all share a single connection without blocking eachother by means of [*pipelining*](http://en.wikipedia.org/wiki/Protocol_pipelining). There's no serialization on request-result or even for a single large message, as the gotalk protocol is frame-based and multiplexes messages over a single connection. This means you can perform several requests at once without having to think about queueing or blocking.

![Diagram of how Gotalk uses connection pipelining](/res/gotalk-pipeline-diagram.png)

#### Simple

Gotalk has a simple and opinionated API with very few components. You expose an operation via "handle" and send requests via "request".

```go
Handle("greet", func(params In) (Out, error) { ... })
// ...
Request("greet", params, &result)
```

#### Practical

Gotalk includes a [JavaScript implementation](https://github.com/rsms/gotalk/tree/master/js) for Web Sockets alongside the full-featured Go implementation, making it easy to build modern web applications. The Gotalk source code also includes [a number of easily-readable examples](https://github.com/rsms/gotalk/tree/master/examples).

```js
handle("greet", function (params, result) { ... });
// ...
request("greet", params, function (err, result) { ... });
```

#### Debuggable

The Gotalk protocol's wire format is byte based for easy on-the-wire inspection of data. It can thus be operated over any reliable byte transport. For example, here's a protocol message representing a request for an operation "hello" with the parameter "world":

    r001005hello00000005world


## By example

As just mentioned earlier, there are a few examples in the `examples` directory demonstrating how you can use Gotalk. But let's explore a simple program right here — a little something written in [Go](http://golang.org/) which demonstrates the use of an operation named "greet":

```go
func server() {
  gotalk.Handle("greet", func(in GreetIn) (GreetOut, error) {
    return GreetOut{"Hello " + in.Name}, nil
  })
  if err := gotalk.Serve("tcp", "localhost:1234"); err != nil {
    log.Fatalln(err)
  }
}

func client() {
  s, err := gotalk.Connect("tcp", "localhost:1234")
  if err != nil {
    log.Fatalln(err)
  }
  greeting := &GreetOut{}
  if err := s.Request("greet", GreetIn{"Rasmus"}, greeting); err != nil {
    log.Fatalln(err)
  }
  log.Printf("greeting: %+v\n", greeting)
  s.Close()
}
```

Let's look at the above example in more detail, broken apart to see what's going on.

We begin by importing the gotalk library together with `log` which we use for printing to the console:

```go
package main
import (
  "log"
  "github.com/rsms/gotalk"
)
```

We define two types: Expected input (request parameters) and output (result) for our "greet" operation:

```go
type GreetIn struct {
  Name string `json:"name"`
}
type GreetOut struct {
  Greeting string `json:"greeting"`
}
```

Registers a process-global request handler for an operation called "greet" accepting parameters of type `GreetIn`, returning results of type `GreetOut`:

```go
func server() {
  gotalk.Handle("greet", func(in GreetIn) (GreetOut, error) {
    return GreetOut{"Hello " + in.Name}, nil
  })
```

Finally at the bottom of our `server` function we call `gotalk.Serve`, which starts a local TCP server on port 1234:

```go
  if err := gotalk.Serve("tcp", "localhost:1234"); err != nil {
    log.Fatalln(err)
  }
}
```

In out `client` function we start by connecting to the server:

```go
func client() {
  s, err := gotalk.Connect("tcp", "localhost:1234")
  if err != nil {
    log.Fatalln(err)
  }
```

Finally we send a request for "greet" and print the result:

```go
  greeting := GreetOut{}
  if err := s.Request("greet", GreetIn{"Rasmus"}, &greeting); err != nil {
    log.Fatalln(err)
  }
  log.Printf("greeting: %+v\n", greeting)

  s.Close()
}
```

Output:

```go
greeting: {Greeting:Hello Rasmus}
```

## Gotalk in the web browser

Gotalk is implemented not only in the full-fledged Go package, but also in a [JavaScript library](https://github.com/rsms/gotalk/tree/master/js). This allows writing web apps talking Gotalk via Web Sockets possible.

```go
// server.go:
package main
import (
  "net/http"
  "github.com/rsms/gotalk"
)
func main() {
  gotalk.HandleBufferRequest("echo", func(in []byte) ([]byte, error) {
    return in, nil
  })
  http.Handle("/gotalk", gotalk.WebSocketHandler(nil, nil))
  http.Handle("/", http.FileServer(http.Dir(".")))
  err := http.ListenAndServe(":1234", nil)
  if err != nil {
    panic("ListenAndServe: " + err.Error())
  }
}
```

In our html document, we begin by registering any operations we can handle:

    <!-- index.html -->
    <body>
    <script type="text/javascript" src="gotalk.js"></script>
    <script>
    gotalk.handle('greet', function (params, result) {
      result({ greeting: 'Hello ' + params.name });
    });
    </script>


But you probably want to have the *browser* send requests to the *server*, so let's send a "echo" request just as our connection opens:

```js
gotalk.connect('ws://'+document.location.host+'/gotalk', function (err, s) {
  if (err) return console.error(err);
  s.request("echo", "Hello world", function (err, result) {
    if (err) return console.error('echo failed:', err);
    console.log('echo result:', result);
  });
});
```

We could rewrite our code like this to allow some UI component to send a request:

```js
var s = gotalk.connect('ws://'+document.location.host+'/gotalk', function (err, s) {
  if (err) return console.error(err);
});

button.addEventListener('click', function () {
  s.request("echo", "Hello world", function (err, result) {
    if (err) return console.error('echo failed:', err);
    console.log('echo result:', result);
  });
});
```

