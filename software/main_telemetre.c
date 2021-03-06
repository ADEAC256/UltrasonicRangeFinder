#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


#define iowrite32(v,a)	*((unsigned int*)(a)) = (v)
#define ioread32(a)		(*((unsigned int*)(a)))


/* Cyclone V FPGA devices */
#define HW_REGS_BASE  0xff200000
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

#define TELEMETRE_BASE 0x00
#define FPGA_REGS_SPAN 0x100


int main() {
	int fd;
	unsigned int i =0;
	
	unsigned int distance;
	unsigned int distance2;
	
	volatile unsigned int *h2p_lw_telemetre_addr=NULL;
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
	
	// Get the address that maps to the FPGA Telemetre IP 
	h2p_lw_telemetre_addr=(unsigned int *)(h2p_lw_virtual_base + TELEMETRE_BASE);
	
	printf("memory mapped\n");
	
	for (i=0 ; i<1000000 ; i++) {
		//iowrite32(i, io_led);
		sleep(1);
		distance = ioread32(h2p_lw_telemetre_addr);
		distance2 = *(unsigned int*)h2p_lw_telemetre_addr;
		printf("distance = %x (hexa) \t distance :  %d cm \n", distance&0xFFFF, distance2&0xFFFF);
	}
	
	//fermer le map
	munmap(h2p_lw_virtual_base, 0x100);
	
	
	printf("memory unmapped\n");
	
	close(fd);
	return( 0 );
}