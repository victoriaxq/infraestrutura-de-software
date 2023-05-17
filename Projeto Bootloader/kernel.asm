; Jogo da forca em Assembly x86

org 0x7e00
jmp 0x0000:main

section .data
message db 'JOGO DA FORCA', 0
prompt1 db 'Digite uma letra: ', 0
palavra db 'CASA', 0
error db 'ERRO', 0
miss db 'tente outra vez', 0
mask db '_ _ _ _', 0
hangman1 db '       __________________"', 0
hangman2 db '             |       _||_       ', 0
hangman3 db '             |      (____)      ', 0
hangman4 db '             |      __||__      ', 0
hangman5 db '             |     /|    |\     ', 0
hangman6 db '             |    //|    |\\    ', 0
hangman7 db '             |   // |____| \\   ', 0
hangman8 db '             |      |    |      ', 0
hangman9 db '             |      | /\ |      ', 0
hangman0 db '             |      |/  \|      ', 0
hangman10 db  '             |      | /         ', 0
hangman11 db '             |      |/          ', 0
hangman12 db '             |                  ', 0
hangman13 db '             |     /|    |     ', 0
hangman14 db '             |    //|    |    ', 0
hangman15 db '             |   // |____|    ', 0
hangman16 db '             |      |____|    ', 0
over db 'PERDESTES!!', 0
win1 db 'GANHASTES!!', 0
X times 1 db 0
Y times 5 db 0 ;letras certas

section .code

; função principal
main:
   
    xor ax, ax    ;limpando ax
    mov ds, ax    ;limpando ds
    mov es, ax    ;limpando es
    mov cx, 0     ;tentativas

    mov ah, 0 ;setando a tela
    mov al, 2 
    int 10h  

    ; exibir a mensagem de boas-vindas
    mov si, message
    call prints
    call endl

    ; inicializar a máscara da palavra
    mov si, mask
    call prints
    call endl

    ; display prompt for guessing word
    mov si, prompt1
    call prints
    call endl

  ; inicializar o boneco
    mov si, hangman1
    call prints
    call endl

    mov si, hangman2
    call prints
    call endl

    mov si, hangman3
    call prints
    call endl

    mov si, hangman4
    call prints
    call endl

    mov si, hangman5
    call prints
    call endl

    mov si, hangman6
    call prints
    call endl

    mov si, hangman7
    call prints
    call endl

    mov si, hangman8
    call prints
    call endl

    mov si, hangman9
    call prints
    call endl

    mov si, hangman0
    call prints
    call endl

    ; loop para o jogo
    jmp play

hang1: ;boneco sem perna direita
    mov si, hangman1
    call prints
    call endl

    mov si, hangman2
    call prints
    call endl

    mov si, hangman3
    call prints
    call endl

    mov si, hangman4
    call prints
    call endl

    mov si, hangman5
    call prints
    call endl

    mov si, hangman6
    call prints
    call endl

    mov si, hangman7
    call prints
    call endl

    mov si, hangman8
    call prints
    call endl

    mov si, hangman10
    call prints
    call endl

    mov si, hangman11
    call prints
    call endl
    
    jmp play

hang2: ;boneco sem pernas
    mov si, hangman1
    call prints
    call endl

    mov si, hangman2
    call prints
    call endl

    mov si, hangman3
    call prints
    call endl

    mov si, hangman4
    call prints
    call endl

    mov si, hangman5
    call prints
    call endl

    mov si, hangman6
    call prints
    call endl

    mov si, hangman7
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    jmp play

hang3: ;boneco sem pernas e sem braco direito
    mov si, hangman1
    call prints
    call endl

    mov si, hangman2
    call prints
    call endl

    mov si, hangman3
    call prints
    call endl

    mov si, hangman4
    call prints
    call endl

    mov si, hangman13
    call prints
    call endl

    mov si, hangman14
    call prints
    call endl

    mov si, hangman15
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    jmp play

hang4: ;boneco sem braco nenhum
    mov si, hangman1
    call prints
    call endl

    mov si, hangman2
    call prints
    call endl

    mov si, hangman3
    call prints
    call endl

    mov si, hangman4
    call prints
    call endl

    mov si, hangman8
    call prints
    call endl

    mov si, hangman8
    call prints
    call endl

    mov si, hangman16
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    jmp play

hang5: ;only cabeca
    mov si, hangman1
    call prints
    call endl

    mov si, hangman2
    call prints
    call endl

    mov si, hangman3
    call prints
    call endl

    mov si, hangman4
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    mov si, hangman12
    call prints
    call endl

    jmp play
    
;função para verificar se o caractere é válido
check:
  mov si, X
  mov di, palavra

   lodsb
   cmploop:

    cmp byte[di], 0    ; 0 é o valor atribuido ao final de uma string
    je incorrect       ; Se for o final da string, incorreto
    cmp al, byte[di]
    je match
    clc
    inc di
    jmp cmploop

; exibir mensagem de erro
 incorrect:
    inc cx
    mov si, miss
    call prints
    call endl
    cmp cx, 1
    je hang1
    cmp cx, 2
    je hang2
    cmp cx, 3
    je hang3
    cmp cx, 4
    je hang4
    cmp cx, 5
    je hang5
    cmp cx, 6
    je gameover

  ; atualize a máscara
match:

  push ax ;guarda valor de ax na pilha
  mov di, Y ; di recebe o endereço de memória da string Y
  mov si, Y ; si recebe o endereço de memória da string Y
  
  blank_space: ;procura espaço vazio em Y
    lodsb; carrega o valor da posição atual de si na memória em al e incrementa si
    cmp al, 0
    je end_blank_space
    inc di
    jmp blank_space

  end_blank_space:
    pop ax ; recupera o valor de ax da pilha
    stosb  ; armazena o valor em al no endereço atual de di e incrementa di
    mov di, palavra  ; di recebe o endereço de memória da string "palavra"
    xor ax, ax
    push ax ; guarda o valor zero de ax na pilha

  loop3:
    mov si, Y
    cmp byte[di], 0
    je endloop3

    loop4:
      lodsb ; carrega o valor da posição atual de si na memória em al e incrementa si
      cmp al, 0
      je print_underline
      cmp byte[di], al
      je printa
      jmp loop4

    endloop4:
      inc di
      jmp loop3
      
  endloop3:
    pop ax
    cmp ax, 0
    je win
    call endl
    jmp play

  print_underline:
    pop ax ;tira da pilha
    inc ax ;aumenta contador 
    push ax ;põe na pilha
    mov al, 95
    call putchar
    jmp endloop4

; loop principal de jogo
play:
    mov di, X
    ; ler uma letra
    call getchar
    ; verificar se o caractere é válido
    call check
    ; atualizar a máscara da palavra
 
putchar:    ;Printa um caractere na tela, pega o valor salvo em al
 mov ah, 0x0e
 int 10h
 ret

endl:       ;Pula uma linha, printando na tela o caractere que representa o /n
  mov al, 0x0a          ; line feed
  call putchar
  mov al, 0x0d          ; carriage return
  call putchar
  ret
   
getchar:    ;Pega o caractere lido no teclado e salva em al
 mov ah, 0x00
 int 16h
 mov di, X
 stosb
 ret

delchar:    ;Deleta um caractere lido no teclado
 mov al, 0x08          ; backspace
 call putchar
 mov al, ' '
 call putchar
 mov al, 0x08          ; backspace
 call putchar
 ret

prints:             ; mov si, string
   .loop:
       lodsb           ; bota character apontado por si em al
       cmp al, 0       ; 0 é o valor atribuido ao final de uma string
       je .endloop     ; Se for o final da string, acaba o loop
       call putchar    ; printa o caractere
       jmp .loop       ; volta para o inicio do loop
        .endloop:
   ret

printa:
call putchar
inc di
jmp loop3


; sair do programa
gameover:
  mov ah, 0 ;setando a tela
  mov al, 2
  int 10h  

  mov ah, 0xb ;numero da chamada
  mov bh, 0   ; ID da paleta de cores
  mov bl, 4  ;cor desejada
  int 10h  

  mov si, over
  call prints
  mov ax, 4C00h
  int 21h
  jmp $
  
win:
  mov ah, 0 ;setando a tela
  mov al, 2
  int 10h  

  mov ah, 0xb ;numero da chamada
  mov bh, 0   ; ID da paleta de cores
  mov bl, 9  ;cor desejada
  int 10h  

  mov si, win1
  call prints