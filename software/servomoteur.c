#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


#define iowrite32(v,a)	*((unsigned int*)(a)) = (v)
#define ioread32(a)		(*((unsigned int*)(a)))


/* Cyclone V FPGA devices */
#define HW_REGS_BASE  0xff200000
#define HW_REGS_SPAN ( 0x04000000 ) //taille max autorisés pour la mémoire
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

#define SERVO_BASE 0x02


#define FPGA_REGS_SPAN 0x1000 //taille réelle autorisés pour la mémoire ->utilisé dans mmap


int main() {
	int fd;
	unsigned int i =0;
	unsigned int writtenvalue;
	//unsigned int degre;
	
	volatile unsigned int *h2p_lw_servomoteur_addr=NULL;
    void *h2p_lw_virtual_base;
	
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
	
	printf("/dev/mem opened\n");
	
	//ouvrir le map
	h2p_lw_virtual_base = mmap( NULL, FPGA_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );
	
	if( h2p_lw_virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}
	
	// Get the address that maps to the FPGA Servomoteur IP 
	h2p_lw_servomoteur_addr=(unsigned int *)(h2p_lw_virtual_base + SERVO_BASE);
	
	printf("memory mapped\n");

	i=1;
	unsigned int flag = 0;
	while (i>0){
		if (!flag){
			if (i<251) i+=10;
			else flag = 1;
		}
		else {
			if (i>1) i-=10;
			else flag = 0;
		}
		iowrite32(i,h2p_lw_servomoteur_addr);
		writtenvalue = ioread32(h2p_lw_servomoteur_addr);
		printf("writtenvalue = %x \n", writtenvalue&0xFFFF);
		sleep(1);
	}
	
		// pour 16 bits, le range est de 2.75 mdegrés

	/*for (i=0 ; i<65535 ; i++) {
		sleep(1);
		iowrite32(i*10,h2p_lw_servomoteur_addr);
		writtenvalue = ioread32(h2p_lw_servomoteur_addr);
		printf("value = 0x%32.32X \n mdegres test : %x \n i:%d \n", writtenvalue&0xFFFF,*h2p_lw_servomoteur_addr,i*10);

	} */
	
	//fermer le map
	munmap(h2p_lw_virtual_base, 0x1000);
	
	
	printf("memory unmapped\n");
	
	close(fd);
	return( 0 );
}