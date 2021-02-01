library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity servomoteur is
	Generic (  Fclock : positive := 50E6;-- System Clock Freq in Hertz
				N_pos : integer := 4; -- Nombre de bits
				Working_range : positive := 500; -- range = 2ms 
				Working_freq : positive := 50);  -- 20ms 
	port(
		CLK,RST_n: in std_logic; -- clock = 50 mHz
		position : in std_logic_vector (N_pos-1 downto 0); -- 4 bits bc DE0-Nano
		commande : out std_logic --signal d'un bit co à l'entrée du servomoteur
		);
end entity;

architecture RTL of servomoteur is

constant Freq_need : positive := (2**N_pos-1) * Working_range; -- freq de resolution -> range / 2^N
constant resolution_cpt : positive := Fclock  / Freq_need; -- nb coups de clock pour 1.25E-4

constant Divisor_clk : positive := Fclock  / Working_freq; --nombre de coup de clock avant 20 ms

constant min_val : positive := 2000;
constant min_time : positive := Fclock  / min_val; -- 25k nb de coup de clock min

signal reg_pos : integer; 
signal cmpt : integer range 0 to Divisor_clk; -- 0 à 2.5E9 mdrrr pour atteindre 20 ms
-- 2* 1,25E-4 + 0.5ms

	begin
	
	reg_pos <= to_integer(unsigned(position))*resolution_cpt + min_time; --exprimé en fonction du nb coups de clock
	
	process (CLK, RST_n)
	begin 
	
	if (RST_n = '0') then 
		cmpt <= 0;
	elsif rising_edge(CLK) then
		if (cmpt < Divisor_clk ) then 
			cmpt <= cmpt + 1;
		else 
			cmpt <= 0;
		end if;
	end if;
	end process;
	
	commande <= '1' when (cmpt < reg_pos) else '0';

end architecture;