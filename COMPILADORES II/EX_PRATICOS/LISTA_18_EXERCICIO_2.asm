.data
    msgBase:     .asciiz "Entre com a base: "
    msgExpoente: .asciiz "Entre com o expoente: "
    msgRes:      .asciiz "Resultado: "

.text
main:
    # Leitura da base
    li $v0, 4
    la $a0, msgBase
    syscall

    li $v0, 5
    syscall
    move $s0, $v0       # $s0 = base

    # Leitura do expoente
    li $v0, 4
    la $a0, msgExpoente
    syscall

    li $v0, 5
    syscall
    move $s1, $v0       # $s1 = expoente

    # Imprime mensagem "Resultado: "
    li $v0, 4
    la $a0, msgRes
    syscall

    # Chama função potencia(base, expoente)
    jal potencia

    # Exibe resultado em $v0
    move $a0, $v0
    li $v0, 1
    syscall

    # Encerrar programa
    li $v0, 10
    syscall


# -------------------------------
# Função potencia
# Entrada: $s0 = base, $s1 = expoente
# Saída:   $v0 = base^expoente
# -------------------------------
potencia:
    # Salva $ra e $s1 na pilha
    addi $sp, $sp, -8
    sw $ra, 0($sp)
    sw $s1, 4($sp)

    # Caso base: expoente == 0 => retorna 1
    beq $s1, $zero, caso_base

    addi $s1, $s1, -1     # decrementa expoente
    jal potencia          # chamada recursiva
    mul $v0, $s0, $v0     # resultado parcial

    j fim_potencia

caso_base:
    li $v0, 1

fim_potencia:
    # Restaura registradores
    lw $ra, 0($sp)
    lw $s1, 4($sp)
    addi $sp, $sp, 8
    jr $ra
