# metalDetector
Authors: 
Søren Møller Christensen 
Nicklas Stubkjær Holm

main file is main.c
This code can detect metals by analyzing the signal amplitude output of a
simple LC amplifier circuit. This is due to the magnectic field of the coil
inducing the nearby metal, which will change the permeability of the core of the coil, 
and thus the frequency and amplitude of the LC's circuit signal will change aswell, as we have

f = 1/(2*pi*sqrt(L*C))

for a first order LC circuit, where
f is the frequency, L is the inductance of the coil and C is the capacitance of the capacitor.
L is dependent on the permeability, and therefore the frequency will change when the field induces a metal.
