library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity servomoteur_avalon is
	Generic (  Fclock : positive := 50E6;-- System Clock Freq in Hertz
				N_pos : integer := 8; -- Nombre de bits
				Working_freq_need : positive := 500; -- range = 2ms -> / de 0.5ms à 2.5ms
				Working_freq : positive := 50);  -- 20ms 
	port(
		CLK,RST_n,chipselect,write_n,read_enable : in std_logic; -- clock = 50 mHz
		commande : out std_logic; --signal d'un bit co à l'entrée du servomoteur
		--Interface avec le bus Avalon
		WriteData : in std_logic_vector (N_pos-1 downto 0); -- 4 bits bc DE0-Nano
		read_data : out std_logic_vector (N_pos-1 downto 0)	-- pour lire la valeur écrite 			
		
		);
end entity;

architecture RTL of servomoteur_avalon is

constant Freq_need : positive := (2**N_pos-1) * Working_freq_need; -- freq de resolution -> range / 2^N
constant resolution_cpt : positive := Fclock  / Freq_need; -- nb coups de clock pour 1.25E-4

constant Divisor_clk : positive := Fclock  / Working_freq; --nombre de coup de clock avant 20 ms

constant min_val : positive := 2000; --freq pour t=0.5ms
constant min_time : positive := Fclock  / min_val; -- 25k nb de coup de clock min
--constant min_val : integer := 0; --freq pour t=0.5ms --2000
--constant min_time : integer := 0;

signal reg_pos : integer; 
signal cmpt : integer range 0 to Divisor_clk; -- 0 à 2.5E9 mdrrr pour atteindre 20 ms

signal reg_writedata : std_logic_vector (N_pos-1 downto 0); 

	begin
	
	--Very Simple Avalon READ
	process(clk, rst_n)
	begin
		if rst_n = '0' then 
			Read_data <= (others => '0');
		elsif rising_edge(clk) then 	
			if (chipselect = '1') and (read_enable = '1') then 
				Read_data <= reg_writedata;
			end if; 
		end if; 
	end process;
	
	--Very Simple Avalon bus WRITE
	process(CLK, RST_n)
	begin
		if RST_n = '0' then 
			reg_writedata <= (others => '0');
		elsif rising_edge(CLK) then 	
			if (chipselect = '1') and (write_n = '0') then 
				reg_writedata <= WriteData;
			end if; 
		end if; 
	end process;
	
	
	reg_pos <= to_integer(unsigned(reg_writedata))*resolution_cpt + min_time; --exprimé en fonction du nb coups de clock
	-- 2* 1,25E-4 + 0.5ms
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