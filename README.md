# Cardano-grid
Decoder according to the method of the Cardano lattice.

# HOWTO
First of all you must to create a files with encrypted text and with Cardano grid.
To get the decrypted text, you need to run the following command:
<code>
./zhi_two -k keys.txt -t text.txt
</code>

Then you will see the following:


    Hello. This is Kardano show!
	Nice to meet you! Let's start!
	Your key file will be: keys.txt
	Your plaintext file will be: text.txt
  
    Your key: 
        0100
        0010
        0100
        0010

    Your encr phrase: zуzzxxxxzz zxбxяzтzz xбxzzеzxxxxyyyycрccyбyрccаcаyтycкccyyyycc,c
    Your plain text: у тебя брак,брат>

At the output of the program you will receive a `plaintext.txt` file with the decrypted text.
