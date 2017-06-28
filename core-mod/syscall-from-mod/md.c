#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/unistd.h> 
#include <linux/string.h> 
#include <linux/cdev.h> 
#include <linux/fs.h> 
int write_call( int fd, const char* str, int len ) { 
   long __res; 
   __asm__ volatile ( "int $0x80": 
      "=a" (__res):"0"(__NR_write),"b"((long)(fd)),"c"((long)(str)),"d"((long)(len)) ); 
   return (int) __res; 
} 
void do_write( void ) { 
   char *str = "write-call string!"; 
   int len = strlen( str ) + 1, n; 
   printk( KERN_INFO "string for write length = %d\n", len ); 
   n = write_call( 1, str, len ); 
   printk( KERN_INFO "write return : %d\n", n ); 
} 
int mknod_call( const char *pathname, mode_t mode, dev_t dev ) { 
   long __res; 
   __asm__ volatile ( "int $0x80": 
      "=a" (__res): 
      "a"(__NR_mknod),"b"((long)(pathname)),"c"((long)(mode)),"d"((long)(dev)) 
   ); 
   return (int) __res; 
}; 
void do_mknod( void ) { 
   char *nam = "ZZZ"; 
   int n = mknod_call( nam, S_IFCHR | S_IRUGO, MKDEV( 247, 0 ) ); 
   printk( KERN_INFO "mknod return : %d\n", n ); 
} 
int getpid_call( void ) { 
   long __res; 
   __asm__ volatile ( "int $0x80":"=a" (__res):"a"(__NR_getpid) ); 
   return (int) __res; 
}; 
void do_getpid( void ) { 
   int n = getpid_call(); 
   printk( KERN_INFO "grtpid return : %d\n", n ); 
} 
static int __init hello_init( void ) { 
   printk( KERN_INFO "======== start module ==========\n" ); 
   do_write(); 
   do_mknod(); 
   do_getpid(); 
   return -1;   // конструкция только для тестирования
} 
module_init( hello_init ); 