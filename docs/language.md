---
layout: default
overview: true
---

# L-Code language
## Introduction
The L-Code language is an attempt to provide a language able to describe
several Lindenmayer's systems.

## Programming in L-Code
### Structure of a L-Code program

An L-Code program look like almost any Lindenmayer's system textual representation.
The following example is the [Koch curve](https://en.wikipedia.org/wiki/L-system#Example_4:_Koch_curve)
written in L-Code:
{% highlight bash %}
axiom: -F;
iteration: 4;
distance: 10.;
angle: 90.;

F -> F+F-F-F+F;
{% endhighlight %}
This code is very similar to the Koch curve pseudo-code on Wikipedia.

### L-Code program's elements
#### Variables

| Name      | Value type       | Description                                                 |
|-----------|------------------|-------------------------------------------------------------|
| axiom     | Modules          | Modules from which to begin construction                    |
| iteration | unsigned integer | Iteration count                                             |
| distance  | unsigned float   | The distance that the turtle moves when 'forward' is called |
| angle     | float            | The angle that the turtle rotates when '+' or '-' is called |
| 
#### Modules
#### Aliases
#### Rewrite rules

## References

 - http://algorithmicbotany.org papers: http://algorithmicbotany.org/papers/abop/
  