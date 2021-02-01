--test tb de la mae 

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;


entity servomoteur_tb is
end entity;

architecture bench of servomoteur_tb is
  
    signal CLK,n_reset :std_logic; -- clock = 50 mHz
	signal posi : std_logic_vector (3 downto 0); -- 4 bits bc DE0-Nano
	signal com : std_logic;


BEGIN

Clok: process
	begin
		while now <= 61 ms loop
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
	
	posi <= (others => '0'); --position = 0;
	
	-- Génération du RST
	n_reset <= '0';
	wait for 10 ns;
	n_reset <= '1';
	wait for 10 ns;
	
	wait for 20 ms;
	
	posi <= "0111"; --position = 7;
	
	wait for 20 ms;
	
	posi <= "1111"; --position = 15;
	
	wait for 20 ms;
	
	wait;
	
  end process;

  
  D1: entity work.servomoteur
    port map (
		CLK => CLK,
		RST_n => n_reset,
		position => posi ,
		commande => com
	  );
	



end architecture;