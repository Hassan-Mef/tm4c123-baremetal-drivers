target remote localhost:3333

monitor reset halt

echo \n====================================\n
echo Connected to target.\n
echo Target reset and halted.\n
echo ====================================\n

# Optional: Display current location
info registers pc sp

break startup.s:23