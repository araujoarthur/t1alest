import argparse
import copy
import random
import math
import time
import struct

all_chars = [chr(i) for i in range(97, 123)]
KEEP_WEIGHT = 95/100
INSERT_SEQ_WEIGHT = 2/100

NEW_LINE_BYTE = bytes([10])

def tc_dict_normal(seq):
   
   searchstr_size = (1 << 10)*(-1)
   while searchstr_size < len(seq):
      searchstr_size = random.randint(1,1024)
   selectable_chars = [c for c in all_chars if c not in seq]
   #print('Sequence: ', seq)
   #print('Selectable chars', selectable_chars)
   #print('Search Stz Size', searchstr_size)
   search_str = []

   # Preenche a array search_str com caracteres aleatorios sem incluir a sequencia
   for i in range(searchstr_size):
      random_index = random.randint(0, len(selectable_chars)-1)
      search_str.append(selectable_chars[random_index])

   # Insere aleatoriamente os caracters que existem na sequencia
   if len(seq) > 1:
      for i in range(len(search_str)):
         dice = random.uniform(0, 1)
         if dice > KEEP_WEIGHT:
            rand_seqchar = seq[random.randint(0, len(seq) - 1)]
            search_str[i] = rand_seqchar;

   max_occurences = math.floor(searchstr_size/len(seq))


   occurence_count = 0
   occurence_positions = []
   new_list = []

   while occurence_count == 0:
      new_list = []
      occurence_count = 0
      occurence_positions = []
      for i in range(0, len(search_str)):
         if occurence_count > 1 and occurence_count > max_occurences:
            break

         dice = random.uniform(0, 1)
         if dice < INSERT_SEQ_WEIGHT:
            insert_pos = len(new_list)
            new_list.extend(seq)
            occurence_count += 1
            occurence_positions.append(insert_pos)
            new_list.append(search_str[i])
         else:
            new_list.append(search_str[i])
   
   return {
   'seq': seq,
   'oc': occurence_count,
   'oc_pos': occurence_positions,
   'searchstr': new_list
   }
   
def build_tc_file(tcdict):
   databuffer = bytearray()
   databuffer += struct.pack(">I", len(tcdict['searchstr'])) # 4 byte integer
   databuffer +=  ''.join(tcdict['searchstr']).encode('ascii')
   
   databuffer += struct.pack(">I", len(tcdict['seq']))
   databuffer += ''.join(tcdict['seq']).encode('ascii')

   databuffer += struct.pack(">I", tcdict['oc'])
   for i in tcdict['oc_pos']:
      databuffer += struct.pack(">I", i)

   
   return databuffer

      
def save_tc_file(buffer):
   t = int(time.time())
   with open(f"search-{t}.tc", "wb") as f:
      f.write(buffer)





parser = argparse.ArgumentParser(prog='Gerador de Test Case', 
                                 description="Esta script é utilizada para gerar um testcase que pode ser utilizado pela script final")

parser.add_argument("-s", "--sequencia", type=str, help="Sequencia a ser utilizada nos casos de teste")

args = parser.parse_args()

sequence = []
test_cases = []

if args.sequencia:
   sequence = list(copy.copy(args.sequencia))
else:
   s_len = random.randint(1,8)
   for i in range(s_len):
      char_ascii_code = random.randint(97, 122)
      sequence.append(chr(char_ascii_code))


tc_dict = tc_dict_normal(sequence)
print(f'Occurence Counts: {tc_dict['oc']}')
print(f'Occurence Positions: {tc_dict["oc_pos"]}')
print(f'Sequence: {tc_dict['seq']}')

bf = build_tc_file(tc_dict)
save_tc_file(bf)

