#! /usr/bin/python
import glob
import subprocess

exe = './paralel'
indir = './testein'
outdir = './testeout'
outokdir = indir
ctime = '/usr/bin/time'
fmt = "Exit Status: %x\nCPU %%: %P\nMemory:\n Unshared: %D\t\t\tAvg Total Mem: %K\n Major Page Faults: %F\t\tMinor Page Faults: %R\n No. Swaps Out of Mem: %W\tNo. Invol Context Swiches: %c\tNo. Vol Context Switches: %w\nTime:\n Realtime: %E\t\tSystem Time: %S\t\tUser Time: %U"
diff = '/usr/bin/diff'

output = 'outsr.txt'

def main():
	of = open(output, 'wb')
	for fp in glob.glob('./%s/out*.txt' % indir):
		fname = fp.split('/')[-1]
		fout = fname
		fn = fout.split('.')[0]
		if fn.split('_')[-1] == 'detaliu':
			continue
		
		n, v, t = fn[3:].split('_')
		print fn, n, v, t
		fin = '%s/in%s_%s.txt' % (indir, n, v)
		fout = '%s/%s' % (outdir, fout)
		of.write(fn+'\n')
		of.flush()
		com = [ctime, '-f', fmt, exe, t, fin, fout]
		#print ' '.join(com)
		r = subprocess.call(com, stdout = of, stderr = of)
		of.flush()
		of.write('\n\n\n')
		print 'Time ', r
		fok = '%s/%s' % (outokdir, fname)
		com = [diff, fout, fok]
		r = subprocess.call(com, stderr = subprocess.STDOUT)
		print 'Diff ', r
		


if __name__ == '__main__':
	main()
