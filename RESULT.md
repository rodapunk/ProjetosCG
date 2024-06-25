Obs.: para uma melhor visualização de algumas imagens, basta clicar nela.

# Tarefa M1

Link: https://github.com/rodapunk/ProjetosCG/tree/main/Hello3D

<img src="https://github.com/rodapunk/ProjetosCG/blob/main/images/01-tarefa_m1.png">

# Tarefa M2

Link: https://github.com/rodapunk/ProjetosCG/tree/main/Hello3DCube

<img src="https://github.com/rodapunk/ProjetosCG/blob/main/images/02-tarefa_m2.gif">

Dois cubos renderizados em posições diferentes e controlados sobre o plano dos eixos X e Z com as teclas WASD, enquanto que ao longo do eixo Y utilizam-se as teclas I e J. Também é possível alterar a escala dos cubos com as teclas ] e [. Para que os cubos rotacionem em torno de um dos eixos X, Y ou Z utilizam-se, respectivamente, as teclas X, Y ou Z. Por fim, para que os cubos girem em torno dos três eixos ao mesmo tempo utiliza-se a tecla Q.

# Tarefa M3

Link: https://github.com/rodapunk/ProjetosCG/tree/main/HelloTexturesCubeObj

<img src="https://github.com/rodapunk/ProjetosCG/blob/main/images/03-tarefa_m3.gif">

Um modelo 3D renderizado a partir de um arquivo OBJ com as informações de vértices, vetores normais e coordenadas de textura. Também foi lido um arquivo MTL e impresso no console o nome do arquivo da textura a ser futuramente aplicada na superfície do material para modificar a sua aparência difusa.

# Tarefa M4

Link: https://github.com/rodapunk/ProjetosCG/tree/main/HelloPhong

<img src="https://github.com/rodapunk/ProjetosCG/blob/main/images/04-tarefa_m4.gif">

Três objetos renderizados os quais foram lidas as informações dos vetores normais nos arquivos .obj e configuradas no vertex shader. Também foram lidas as informações dos coeficientes de iluminação ambiente, difusa, especular e o expoente de reflexão especular nos arquivos .mtl e configuradas no fragment shader. Por fim, foi implementado em GLSL, no fragment shader, o cálculo das parcelas ambiente, difusa e especular do modelo de iluminação de Phong.

# Tarefa M5

Link: https://github.com/rodapunk/ProjetosCG/tree/main/HelloPhongCamera

<img src="https://github.com/rodapunk/ProjetosCG/blob/main/images/05-tarefa_m5.gif">

Implementada a câmera em primeira pessoa com o controle de movimento pelas teclas WASD no plano x-z, pelas teclas CTRL e SPACE no plano y e controle de rotação pelo mouse.

# Tarefa M6

Link: https://github.com/rodapunk/ProjetosCG/tree/main/HelloParametric

<img src="https://github.com/rodapunk/ProjetosCG/blob/main/images/06-tarefa_m6.gif">

Implementadas duas funções que criam vetores de posição para que os objetos possam percorrer uma trajetória no espaço, sejam estas trajetórias: retangular no plano xz e aleatória. Uma terceira função escreve em um arquivo txt estes vetores e uma quarta função carrega estes vetores de posição, em uma variável 'positions' do objeto, a partir do arquivo txt. O objeto planeta terra percorre uma trajetória retangular no plano xz em torno do objeto Suzanne, enquanto que o objeto bola percorre uma trajetória aleatória, ambas cíclicas.

# Tarefa Final

Link: https://github.com/rodapunk/ProjetosCG/tree/main/HelloParametricBezier

<img src="https://github.com/rodapunk/ProjetosCG/blob/main/images/07-tarefa_m7.gif">

Implementada a curva paramétrica de Bézier, que é uma curva suave e contínua definida por um conjunto de pontos de controle, onde cada ponto da curva é calculado como uma combinação ponderada desses pontos usando polinômios como funções de interpolação. O objeto planeta terra percorre uma trajetória circular no plano xz em torno do objeto Suzanne, enquanto que o objeto bola percorre uma trajetória aleatória, ambas cíclicas.
