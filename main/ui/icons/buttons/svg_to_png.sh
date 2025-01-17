for f in *.svg; do
    substr="-svgrepo-com.svg"
    f_reduced=${f%"$substr"}
    ffmpeg -i "$f" -vf scale=100:-1 ${f_reduced}.png
done
