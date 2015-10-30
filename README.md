# Ruby Barcode Reader for Mac
The sample demonstrates how to create a Ruby barcode reader on Mac OS X 10.10.5 with [Dynamsoft Barcode C/C++ SDK][1] (DBR).

Prerequisites
-------------
[Dynamsoft Barcode SDK][2]

How to Build and Run
--------------------
1. Generate **Makefile** with

    ```
    ruby extconf.rb
    ```
2. Open **Makefile** and change configurations.
    * Dynamsoft Barcode include directory:

    ```
    dbrdir = /Applications/Dynamsoft/Barcode\ Reader\ 3.0\ Trial/Include

    INCFLAGS = -I. -I$(arch_hdrdir) -I$(hdrdir)/ruby/backward -I$(hdrdir) -I$(srcdir) -I$(dbrdir)
    ```
    * Dynamsoft Barcode dylib:

    ```
    LIBS = $(LIBRUBYARG_SHARED)  -lpthread -ldl -lobjc -lDynamsoftBarcodeReader

    ```
3. Run ``make``. If you get the warnings:

    ```
    ld: warning: directory not found for option '-L/usr/local/lib'
ld: warning: directory not found for option '-L/usr/local/lib'
    ```
    Open **Makefile** and change ``ldflags  = -L. -L/usr/local/lib`` to ``ldflags  = -L. -L/usr/lib``. If there's no warning, you will see ``linking shared-object dynamsoftbarcode.bundle``
4. Run ``sudo make install`` to copy **dynamsoftbarcode.bundle** to **/Library/Ruby/Site/2.0.0/universal-darwin14**
5. Run Ruby script:

    ```
    ruby barcode_reader.rb barcode_file
    ```

Screenshot
----------
![image](http://www.codepool.biz/wp-content/uploads/2015/10/screenshot.png)

References
----------
* http://docs.ruby-lang.org/en/2.1.0/README_EXT.html
* https://silverhammermba.github.io/emberb/c/

Blog
----
[How to Make Ruby Barcode Reader with C++ Barcode SDK on Mac OS X][3]

[1]:http://www.dynamsoft.com/Products/barcode-cpp-api-for-windows.aspx
[2]:http://www.dynamsoft.com/Downloads/Dynamic-Barcode-Reader-Download.aspx
[3]:http://www.codepool.biz/ruby-barcode-reader-c-sdk.html
