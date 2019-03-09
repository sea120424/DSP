
./ngram-count -text corpus_seg.txt -write lm.cnt -order 2
./ngram-count -read lm.cnt -lm bigram.lm -unk -order 2
python3 mapping.py
mkdir result2
./mydisambig -text testdata/seg_1.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/1.txt
./mydisambig -text testdata/seg_2.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/2.txt
./mydisambig -text testdata/seg_3.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/3.txt
./mydisambig -text testdata/seg_4.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/4.txt
./mydisambig -text testdata/seg_5.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/5.txt
./mydisambig -text testdata/seg_6.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/6.txt
./mydisambig -text testdata/seg_7.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/7.txt
./mydisambig -text testdata/seg_8.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/8.txt
./mydisambig -text testdata/seg_9.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/9.txt
./mydisambig -text testdata/seg_10.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/10.txt
