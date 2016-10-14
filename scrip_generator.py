import os
import glob

files = glob.glob('*.c') + glob.glob('*.h')

f = open('scp_script.cmd', 'w')
for fl in files:
    f.write('scp ' + fl + ' up201203463@ssh.alunos.dcc.fc.up.pt:~/Documents/Programacao_Paralela_Floyd\n')

f.close()

os.system('scp_script.cmd')
os.remove('scp_script.cmd')
