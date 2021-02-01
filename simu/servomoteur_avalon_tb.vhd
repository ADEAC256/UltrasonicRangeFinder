--test tb de la mae 

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;


entity servomoteur_avalon_tb is
end entity;

architecture bench of servomoteur_avalon_tb is
  
    signal CLK,n_reset,Chipselect,Write_n,Read_enable :std_logic; -- clock = 50 mHz
	signal posi,Read_data : std_logic_vector (7 downto 0); -- 
	signal com : std_logic;


BEGIN

Clok: process
	begin
		while now <= 81 ms loop
		  CLK <= '0';
		  wait for 10 NS;
		  CLK <= '1';
		  wait for 10 NS;
		end loop;
		wait;
	end process;

-- component instantiation

stim: process 
	begin
	-- Génération du RST

	Write_n <= '1';
	Chipselect <= '0';
	
	n_reset <= '0';
	wait for 10 ns;
	n_reset <= '1';
	wait for 10 ns;
	
	posi <= (others => '0'); --position = 0;
	
	wait for 20 ms;
	
	Chipselect <= '1';
	
	posi <= "00001010"; --position = 10;
	wait for 20 ms;
	
	Write_n <= '0';
	posi <= "10000000"; --position = 128;
	
	wait for 20 ms;
	
	posi <= "11111111"; --position = 255;
	
	wait for 20 ms;
	
	wait;
	
  end process;

  
  D1: entity work.servomoteur_avalon
    port map (
		CLK => CLK,
		RST_n => n_reset, 
		chipselect => Chipselect,
		write_n => Write_n ,
		read_enable => Read_enable,
		commande => com,
		WriteData => posi ,
		read_data => Read_data
	  );
	



end architecture;