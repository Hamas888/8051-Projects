ORG 00H
AGAIN2:
     MOV R6,#0
G:
     MOV R5,#0
AGAIN6 :
     MOV R4,#0
AGAIN5:
     MOV R3,#0
AGAIN4:
  
      MOV P0,#00H 
      MOV R1,#0
      MOV R2,#0
   AGAIN3 : 
      MOV R1,#0
      AGAIN:
      LCALL DIS
      LCALL DIS
      INC R1
      CJNE R1,#10,AGAIN
      INC R2
      CJNE R2,#6,AGAIN3
      INC R3
      CJNE R3,#10,AGAIN4
      INC R4
      CJNE R4,#6,AGAIN5
      INC R5
      CJNE R5,#10,AGAIN6
      INC R6
      CJNE R6,#3,G
      SJMP AGAIN2
      
    DELAY :
   
          MOV R0,#0FFH
       AA:DJNZ R0,AA
          RET
      B1:
         CLR P2.0
         CLR P2.1
         CLR P2.2
         CLR P2.4
         CLR P2.5
         SETB P2.3
         RET
      B2:
         CLR P2.0
         CLR P2.1
         CLR P2.3
         CLR P2.4
         CLR P2.5
         SETB P2.2
         RET 
      B3:
         CLR P2.0
         CLR P2.2
         CLR P2.3
         CLR P2.4
         CLR P2.5
         SETB P2.1
         RET
      B4:
         CLR P2.1
         CLR P2.2
         CLR P2.3
         CLR P2.4
         CLR P2.5
         SETB P2.0
         RET
      B5:
         CLR P2.1
         CLR P2.2
         CLR P2.3
         CLR P2.0
         CLR P2.5
         SETB P2.4
         RET
      B6:
         CLR P2.1
         CLR P2.2
         CLR P2.3
         CLR P2.4
         CLR P2.0
         SETB P2.5
         RET
      DIS:
      
      MOV R7,#0FFH
      AO:
      MOV DPTR,#100H 
      MOV A,R1
      MOVC A,@A+DPTR
      LCALL B1
      MOV P0,A
      LCALL DELAY
      MOV DPTR,#150H 
      MOV A,R2
      MOVC A,@A+DPTR
      LCALL B2
      MOV P0,A
      LCALL DELAY
      MOV DPTR,#100H 
      MOV A,R3
      MOVC A,@A+DPTR
      LCALL B3
      MOV P0,A
      LCALL DELAY
      MOV DPTR,#150H 
      MOV A,R4
      MOVC A,@A+DPTR
      LCALL B4
      MOV P0,A
      LCALL DELAY
      MOV DPTR,#100H 
      MOV A,R5
      MOVC A,@A+DPTR
      LCALL B5
      MOV P0,A
      LCALL DELAY
      MOV DPTR,#150H 
      MOV A,R6
      MOVC A,@A+DPTR
      LCALL B6
      MOV P0,A
      LCALL DELAY
      DJNZ R7,AO
      RET
ORG 100H
DB 10010000B
DB 11010111B
DB 10001100B
DB 10000101B
DB 11000011B
DB 10100001B
DB 10100000B
DB 10010111B
DB 10000000B
DB 10000011B
ORG 150H
DB 10010000B
DB 11010111B
DB 10001100B
DB 10000101B
DB 11000011B
DB 10100001B                 
END