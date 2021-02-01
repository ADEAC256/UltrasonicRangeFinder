# Ultrasonic Range Finder

The purpuse of this project is to map the space using an ultrasound rangefinder and a servomotor. Using the principle of a 2D radar, a display will have to be made on a screen. The display will be on a terminal with a message giving the distance and angle between the nearest object and the ultrasonic radar.

In this project, the behaviour of the ultrasonic rangefinder and the servomotor was first described in VHDL. Then, using the Cortex A9, the system was programmed in C (with mmap) to rotate the servomotor and acquire data regularly with the ultrasound rangefinder.

VHDL sources are in the /src folder. C sources are in /software. To validate the behavior of our IPs, we have carried out test benches that are in the /simu folder.

#### Made by Alexandra Deac & Clément Roger - Polytech Sorbonne EISE4 
