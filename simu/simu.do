vlib work

vcom -93 ../src/servomoteur_avalon.vhd
vcom -93 servomoteur_avalon_tb.vhd

vsim servomoteur_avalon_tb(bench)

add wave *

run -all