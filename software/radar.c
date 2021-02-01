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

#define TELEMETRE_BASE 0x00
#define SERVO_BASE 0x02


#define FPGA_REGS_SPAN 0x1000 //taille réelle autorisés pour la mémoire ->utilisé dans mmap

volatile unsigned int *h2p_lw_servomoteur_addr=NULL;
volatile unsigned int *h2p_lw_telemetre_addr=NULL;

void print_min(unsigned int min,unsigned int deg){
	printf("distance min : %d cm \n", min&0xFFFF);
	printf("degrés : %d° \n", deg&0xFFFF);
}

void set_min(unsigned int i, unsigned int* min, unsigned int* deg){
	unsigned int distance;
	distance = ioread32(h2p_lw_telemetre_addr);
	distance = distance&0xFFFF;
	if (distance < *min){
		*min = distance; 
		*deg = (unsigned int) (i*0.7);
	}
}


int main() {
	int fd;
	
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
	// Get the address that maps to the FPGA Telemetre IP 
	h2p_lw_telemetre_addr=(unsigned int *)(h2p_lw_virtual_base + TELEMETRE_BASE);
	
	printf("memory mapped\n");

	unsigned int min = 256;
	unsigned int deg = 0;
	unsigned int flag = 0;
	unsigned int i = 1;
	
	while (i>0){
		if (!flag){
			if (i<251){ 
				i+=10;
				set_min(i,&min,&deg);
			}
			else {
				flag = 1;
				print_min(min,deg);
				min= 256;
				deg = 0;
			}
		}
		else {
			if (i>1) { 
				i-=10;
				set_min(i,&min,&deg);
			}
			else {
				flag = 0;
				print_min(min,deg);
				min= 256;
				deg = 0;
			}
		}
		iowrite32(i,h2p_lw_servomoteur_addr);
		sleep(1);
	}
	
	//fermer le map
	munmap(h2p_lw_virtual_base, 0x1000);
	
	
	printf("memory unmapped\n");
	
	close(fd);
	return( 0 );
}