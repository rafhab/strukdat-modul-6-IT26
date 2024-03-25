#!/bin/bash

# Unduh file dari URL yang diberikan
echo "=== Downloading files ==="
wget -q -O genshin_character.zip "URL_Genshin_Character.zip"
wget -q -O list_character.csv "URL_List_Character.csv"
wget -q -O genshin.zip "URL_Genshin.zip"

# Ekstrak file yang telah diunduh
echo "=== Extracting files ==="
unzip -q genshin_character.zip
unzip -q genshin.zip

# Dekode nama file yang terenkripsi dengan hexadecimal
echo "=== Decoding filenames ==="
for file in *.jpg; do
    decoded_name=$(echo "$file" | xxd -r -p)
    mv "$file" "$decoded_name"
done

# Rename file berdasarkan data karakter dari list_character.csv
echo "=== Renaming files ==="
while IFS=, read -r region nama elemen senjata; do
    mv "$region"_"$nama"_"$elemen"_"$senjata".jpg "$region"_"$nama"_"$elemen"_"$senjata".jpg
done < list_character.csv

# Hitung jumlah pengguna untuk setiap senjata
echo "=== Counting users for each weapon ==="
declare -A weapon_count
for file in *.jpg; do
    senjata=$(echo "$file" | cut -d'_' -f4)
    ((weapon_count[$senjata]++))
done

# Tampilkan jumlah pengguna untuk setiap senjata
for senjata in "${!weapon_count[@]}"; do
    echo "$senjata : ${weapon_count[$senjata]}"
done

# Hapus file yang tidak diperlukan
echo "=== Removing unnecessary files ==="
rm -f genshin_character.zip list_character.csv genshin.zip
