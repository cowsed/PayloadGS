set -e
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 IMG_DIR BLOCK_SIZE" >&2
    exit 33
fi

IMG_DIR=$1
BLOCK_SIZE=$2

cat $IMG_DIR/packets/pkt_* > $IMG_DIR/image.bin
ssdv -d $IMG_DIR/image.bin -l $BLOCK_SIZE $IMG_DIR/image.jpg
magick $IMG_DIR/image.jpg -resize 128x128 $IMG_DIR/thumbnail.jpg
