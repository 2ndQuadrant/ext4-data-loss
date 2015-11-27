import psycopg2
import sys
import time

if __name__ == '__main__':

	conn = psycopg2.connect('host=192.168.1.37 dbname=pgbench user=postgres')
	cur = conn.cursor()

	while True:

		cur.execute('SELECT pg_current_xlog_location() AS xlog, pg_current_xlog_insert_location() AS xlog_insert, pg_current_xlog_flush_location() AS xlog_flush')
		xlog = cur.fetchone()

		print "location %s insert %s flush %s" % xlog
		sys.stdout.flush()

		time.sleep(0.05)
