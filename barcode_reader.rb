require 'dynamsoftbarcode'
include BarcodeReader

if ARGV.length == 0
  puts "Please add a barcode file path!"
else
  $filePath = ARGV[0]
  $results = decodeFile($filePath)
  $results.each do |i|
     puts "#{i}"
  end
  
  puts "Game Over!"
end
