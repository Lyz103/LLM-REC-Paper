# 😊Collaborative Large Language Model for Recommender Systems

## Motivation
The semantic gap between natu
ral language and recommendation tasks is still not well addressed,
 leading to multiple issues such as spuriously correlated user/item
 descriptors, ineffective language modeling on user/item data, in
efficient recommendations via auto-regression,


## Introduction

预训练阶段包括相互正则化的协同或内容LLMs，通过对基于用户/物品互动和文本特征建立的RS特定语料库进行语言建模来学习用户/物品tokens嵌入。具体来说，提出了一种新颖的“软+硬”提示策略，用于对具有异构标记的文档进行有效的语言建模，其中每个文档被分解为包含软（用户/物品）和硬（词汇）标记的提示以及包含同质项标记（用于协同建模）或词汇标记（用于内容建模）的主文本。通过这种策略，两个LLMs的预测头部可以专注于协同和内容信息，从而可以大大增强语言建模的稳定性和有效性。此外，提出了一种随机项重排序策略，用于协同LLM，以忽略项目标记的顺序而不对词汇标记造成负面影响。最后，提出了一种面向推荐的新颖微调策略，其中将带有多项式似然的项预测头部添加到预训练的协同LLM骨干中，以根据从掩码用户互动历史中建立的软+硬提示来预测留出的项目，从而可以高效生成多个项目的推荐而不产生虚构。

## Method
我们使用尖括号表示法 "<user_𝒊>" 和 "<item_𝒋>" 来表示新引入的tokens，用于第𝑖-th用户和第𝑗-th物品，其token ID为 𝑁+𝑖 和 𝑁+𝐼+𝑗，并且不会被分解为atomic token。  
item/user embedding 是可以训练的 vocab的embedding是不可以训练的？？



