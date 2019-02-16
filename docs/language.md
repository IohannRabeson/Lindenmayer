---
layout: default
overview: true
---

# L-Code language
## Introduction
The L-Code language is an attempt to provide a language able to describe
several Lindenmayer's systems.

 - [DONE] Deterministic L-System
 - [DONE] Stochastic L-System
 - [TODO] Contextual L-System
 - [TODO] Parametric L-System

## Programming in L-Code
### Structure of a L-Code program
An L-Code program look like almost any Lindenmayer's system textual representation.
The following example is the [Koch curve](https://en.wikipedia.org/wiki/L-system#Example_4:_Koch_curve)
written in L-Code:
{% highlight bash %}
axiom: F;
iteration: 4;
angle: 90;

F = F+F−F−F+F;
{% endhighlight %}
This code is very similar to the Koch curve pseudo-code on Wikipedia.


### Some examples  
{% highlight bash %}  
axiom: ----F;  
iteration: 6;  
angle: 22.5;  
distance: 50.;  

F -> (0.5) F[+F][-F]F;  
F -> (0.3) F[-F]F;  
F -> (0.2) F[+F]F;  
{% endhighlight %}  
![results]({{ site.url }}/screenshots/Development snapshots/stochastic_01.png)

### L-Code program's elements

#### Constants

| Constant  | Type             | Description                                                 |
|-----------|------------------|-------------------------------------------------------------|
| axiom     | Modules          | Modules from which to begin construction                    |
| iteration | unsigned integer | Iteration count                                             |
| distance  | unsigned float   | The distance that the turtle moves when 'forward' is called |
| angle     | float            | The angle that the turtle rotates when '+' or '-' is called |
