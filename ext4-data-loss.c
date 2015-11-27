#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NFILES 64

/* fdatasync is default on Linux */
#define SYNC(a) fdatasync(a)

static void prepare_files(int nfiles);
static void rewrite_file(int fd);

static void
rewrite_file(int fd)
{
	int i;
	char data[1024];

	/* 1kB of zeroes */
	memset(data, 0, 1024);

	/* write 16MB of data (zeroes) to the file */
	for (i = 0; i < 16 * 1024; i++)
	{
		if (write(fd, data, 1024) != 1024)
		{
			printf("write failed");
			exit(6);
		}
	}
}

static void
prepare_files(int nfiles)
{
	int i, j;
	char fname[1024];

	for (i = 0; i < nfiles; i++)
	{
		int fd;

		sprintf(fname, "%d", i);

		if ((fd = open(fname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
		{
			printf("open failed");
			exit(7);
		}

		rewrite_file(fd);

		/* fdatasync is default on linux */
		if (SYNC(fd) != 0)
		{
			printf("sync failed");
			exit(8);
		}

		if (close(fd) != 0)
		{
			printf("close failed");
			exit(9);
		}
	}
}

void main()
{
	int i, f;

	prepare_files(NFILES);
	f = NFILES;

	/* now create a new file by renaming the oldest one, rewrite and sync */
	while (1)
	{
		int fd;
		char fname_old[1024];
		char fname_new[1024];

		sprintf(fname_old, "%d", f-NFILES);
		sprintf(fname_new, "%d", f);

		if (rename(fname_old, fname_new) != 0)
		{
			printf("rename failed");
			exit(1);
		}

		if ((fd = open(fname_new, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
		{
			printf("open failed");
			exit(2);
		}

		rewrite_file(fd);

		/* fdatasync is default on linux */
		if (SYNC(fd) != 0)
		{
			printf("sync failed");
			exit(3);
		}

		if (close(fd) != 0)
		{
			printf("close failed");
			exit(4);
		}

		printf("file %s\n", fname_new);

		/* sleep for 1 second */
		sleep(1);

		/* next file */
		f += 1;
	}

}
