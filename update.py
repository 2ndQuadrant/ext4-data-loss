import psycopg2
import sys
from multiprocessing import Process

def run_inserts(pid):

	i = 0
	l = 0

	conn = psycopg2.connect('host=192.168.1.37 dbname=pgbench user=postgres')
	cur = conn.cursor()

	while True:

		try:

			cur.execute('UPDATE test_update SET val = (%s::text) WHERE id = %s', (i,pid))
			conn.commit()

			l = i
			i += 1

		except:

			break

	print "%d %d" % (pid, l)


if __name__ == '__main__':

	processes = [Process(target=run_inserts, args=(i,)) for i in range(32)]

	[p.start() for p in processes]

	[p.join() for p in processes]
