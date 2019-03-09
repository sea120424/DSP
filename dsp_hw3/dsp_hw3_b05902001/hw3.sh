./separator_big5.pl corpus.txt > corpus_seg.txt
./separator_big5.pl testdata/1.txt >testdata/seg_1.txt
./separator_big5.pl testdata/2.txt >testdata/seg_2.txt
./separator_big5.pl testdata/3.txt >testdata/seg_3.txt
./separator_big5.pl testdata/4.txt >testdata/seg_4.txt
./separator_big5.pl testdata/5.txt >testdata/seg_5.txt
./separator_big5.pl testdata/6.txt >testdata/seg_6.txt
./separator_big5.pl testdata/7.txt >testdata/seg_7.txt
./separator_big5.pl testdata/8.txt >testdata/seg_8.txt
./separator_big5.pl testdata/9.txt >testdata/seg_9.txt
./separator_big5.pl testdata/10.txt >testdata/seg_10.txt
./ngram-count -text corpus_seg.txt -write lm.cnt -order 2
./ngram-count -read lm.cnt -lm bigram.lm -unk -order 2
python3 mapping.py
mkdir result1
./disambig -text testdata/seg_1.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/1.txt
./disambig -text testdata/seg_2.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/2.txt
./disambig -text testdata/seg_3.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/3.txt
./disambig -text testdata/seg_4.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/4.txt
./disambig -text testdata/seg_5.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/5.txt
./disambig -text testdata/seg_6.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/6.txt
./disambig -text testdata/seg_7.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/7.txt
./disambig -text testdata/seg_8.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/8.txt
./disambig -text testdata/seg_9.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/9.txt
./disambig -text testdata/seg_10.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/10.txt
