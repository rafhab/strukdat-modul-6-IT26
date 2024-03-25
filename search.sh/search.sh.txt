#!/bin/bash

# Inisialisasi pencatatan log
log_file="image.log"
touch "$log_file"

# Pencarian gambar tersembunyi
echo "=== Searching for hidden images ==="
while true; do
    for image in *.jpg; do
        # Ekstrak nilai dari setiap gambar menggunakan steghide
        extracted_txt=$(steghide extract -sf "$image" -p "" 2>/dev/null | grep "extracted")
        if [[ -n "$extracted_txt" ]]; then
            # Jika hasil ekstraksi adalah file txt yang dicari, simpan hasil dekripsi
            # Jika bukan, hapus file txt tersebut
            if [[ "$extracted_txt" == *"txt"* ]]; then
                decrypted_content=$(xxd -r -p "$extracted_txt")
                echo "$decrypted_content" > "$image.txt"
                echo "$(date +'%d/%m/%y %H:%M:%S') [FOUND] $image" >> "$log_file"
                # Temukan url yang dicari, dan hentikan script
                if [[ "$decrypted_content" == *"URL_Yang_Dicari"* ]]; then
                    echo "URL found: $decrypted_content"
                    exit 0
                fi
            else
                rm "$extracted_txt"
            fi
        fi
    done
    sleep 1
done
