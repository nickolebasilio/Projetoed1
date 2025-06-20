# Projetoed1
void insere_produto(produto **lista_produtos)
{
    produto *novo = (produto *)malloc(sizeof(produto));
    if(novo)
    {
        printf("qual o nome do produto que deseja inserir?\n");
        scanf("%s", &(novo->nome));
        setbuf(stdin, NULL);
        printf("qual o codigo do produto?\n");
        scanf("%d", &(novo->codigo));
        printf("qual a quantidade do produto?\n");
        scanf("%d", &(novo->quantidade));
        for(i=0; i<3; i++)
        {
            printf("qual o valor do produto %d?\n", i+1);
            scanf("%f", &(novo->precos[i]));
        }
        novo->prox = *lista_produtos;
        *lista_produtos = novo;

   }
    else
        {
            printf("problema para inserir produto\n");
        }
}

void insere_clientes(cliente **c)
{
   cliente *novo= (cliente*)malloc(sizeof(cliente));
   if(novo)
   {
       printf("qual o id do cliente?\n");
       scanf("%d", &(novo->id_cliente));
       printf("qual o nome do cliente?\n");
       scanf("%s", &(novo->nome));
       setbuf(stdin, NULL);
       novo->pedidos = NULL;
       novo->prox = *c;
       *c = novo;
   }
   else
    printf("problema na alocacao de memoria\n");
}

void insere_vendas(HistoricoVendas **v, int codigo, produto *lista_produtos) //insere no histórico
{
    produto *aux = lista_produtos;

   while((aux != NULL && aux->codigo != codigo))
   {
       aux = aux->prox;
   }
   if (aux == NULL)
    printf("produto nao encontrado\n");
   HistoricoVendas *novo = (HistoricoVendas*)malloc(sizeof(HistoricoVendas));
   if(novo)
   {

       novo->codigo = aux->codigo;
       for(i=0; i<4; i++) //conferir se eh pra pedir pro usuario mesmo
       {
           printf("qual o valor do trimestre %d", i + 1);
           scanf("%d", &(novo->vendas[i]));
       }
       novo->prox = *v;
       *v = novo;
   }

}

void diminui_estoque(produto *lista_produtos, int qtde, int codigo)
{
    produto *aux = lista_produtos;
    while(aux!=NULL)
    {
        if(aux->codigo == codigo)
        {
        if(qtde <= aux->quantidade)
        {
            aux->quantidade = aux->quantidade - qtde;
            printf("estoque atualizado! Estoque atual do produto %d: %d", aux->codigo, aux->quantidade);
        }
        else
            printf("a quantidade em estoque eh insuficiente\n");
            return; //se o produto foi achado, vai parar (return)
        }
        else
            aux = aux->prox;
    }
    printf("produto nao encontrado\n");
}

void processar_pedidos(produto *lista_produtos, cliente *c) //atualiza o estoque, removendo o que foi vendido
{
    cliente *aux_cliente = c;
    while(aux_cliente != NULL)
    {
        pedido *aux_pedido = aux_cliente->pedidos;
        while(aux_pedido != NULL)
        {
            PedidoItem *aux_item = aux_pedido->itens;
            while(aux_item != NULL)
            {
                diminui_estoque(lista_produtos, aux_item->quantidade, aux_item->codigo_produto); //fazer essa funcao
                aux_item = aux_item->prox;

            }
            aux_pedido = aux_pedido->prox; //muda pro prox pedido
        }
        aux_cliente = aux_cliente->prox;
    }
}

void prever_compras(HistoricoVendas *v, produto *p)
{
    int soma = 0, i;
    float media, reposicao;
    produto *aux = p;
    while(v != NULL)
    {
        soma = 0; //conferir
        for(i=0; i<4; i++)
        {
            soma = soma + v->vendas[i];
        }
        media = (float)soma/4;
        while(aux != NULL && aux->codigo != v->codigo)
        {
            aux=aux->prox;
        }
        if(aux != NULL && aux->quantidade < media)
        {
            reposicao = media - (aux->quantidade);
            relatorio_final(reposicao, aux); //manda a sugestao de reposicao pra funcao de relatorio e aux para que possa acessar qualquer info do produto
      }
        v=v->prox;
        aux = p;
    }
}
