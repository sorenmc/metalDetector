# metalDetector
Authors: 
Søren Møller Christensen 
Nicklas Stubkjær Holm

main file is main.c
This code can detect metals by analyzing the signal amplitude output of a
simple LC amplifier circuit. This is due to the magnectic field of the coil
inducing the nearby metal, which will change the permeability of the core of the coil, 
and thus the frequency and amplitude of the LC's circuit signal will change aswell, as we have

![\Large f = \frac{1}{2 \pi\sqrt{LC}}](https://latex.codecogs.com/gif.latex?f%20%3D%20%5Cfrac%7B1%7D%7B2%20%5Cpi%5Csqrt%7BLC%7D%7D)
 
for a first order LC circuit, where
f is the frequency, L is the inductance of the coil and C is the capacitance of the capacitor.
L is dependent on the permeability, and therefore the frequency will change when the field induces a metal.
