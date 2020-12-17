CC=gcc

SRC= FileManagement/FileRW.c FileManagement/Inode.h FileManagement/FileValidantions.h FileManagement/Block.h FileManagement/Superblock.h BootAndReset/DiskManagement.c

file: TARGET=file

file: all
all:
	$(CC) $(SRC) -o $(TARGET).o

clean:
	find . -name "*.h.gch" -type f -delete
	find . -name "*.o" -type f -delete
