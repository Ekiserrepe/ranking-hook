<p align="center">
  <img src="https://github.com/user-attachments/assets/13931549-9b00-4540-b5df-173f58bd104b">
</p>

# Ranking Hook (An example for Xahau based games) 

## Introduction

Ranking is a hook programmed in C for the Xahau blockchain. A hook is a series of rules that enable smart logic in Xahau, considered the smart contracts of Xahau.

Ranking Hook is a Hook that turns your Xahau account into a score ranking manager for games. It's a simple idea so that others can get inspired and make better versions for their needs.


## Explanation


**Attention:** Only use if you are sure of what you are doing on Testnet/Mainnet. You could put your funds at risk. It is recommended to install on new accounts.

This hook was an example to use in conjunction with the DinoXAHur game, a game demo for Xaman that uses Xahau technology to store and manage players' scores and reward those who achieve the best score.

The hook is designed to manage a ranking of 5 positions. In this ranking we will store the address of the 5 best players and their 5 scores. Every time a player beats one or more of the scores, the ranking will be updated with his data thanks to this hook.

The hook also allows you to manage a vault. The vault will be an amount of XAH that the hook will store with each paid game in the DinoXAHur game. The game will ask for a payment of 1 XAH through the Xaman application to try to beat the ranking. The player who manages to reach the first position in the ranking will receive the vault that is available at that moment. It may be that at that moment, the vault only has 1 XAH from the player who has just started the game if he reaches the first place or it may even be the case that he does not receive any XAH if seconds before someone has reached the first place in the ranking.

To register a score you will need to generate an Invoke transaction from the hook account. In this case, the DinoXAHur game does it when it detects that a player has achieved a required score. The Invoke transaction will need the Hook Param “SCORE” with the score and the Hook Param “ADDRESS” with the player's address. How to include this data is explained in detail below.

The hook will count 1 XAH payments to start games and store it in the main hook namespace with the key “VAULT”. The value of VAULT will increase each time there are paying players. The VAULT will be cleared/reset if someone reaches the top ranking position or the RESET parameter is used. More about it later.

This particular hook allows you to add an external account that will “ignore” if you make payments of 1 XAH like all other players. This can serve as an example if in your implementations you need the VAULT counter not to keep growing with the payments of an account because you don't want it to happen (for example, you are performing tests of another type on that account).

If for some fund management error or necessity, we can reset the VAULT counter. To do this we must initiate an Invoke transaction with the Hook Param “RESET” and the value 01. This will remove the current state of the VAULT counter and return it to 0.

If you want to reset the current ranking. You can use an Invoke transaction with the Hook Param “CLEAN” and the value 01. This will reset the rankings with value score 1 and a default address. This can be useful if you have detected abuses in your game or want to restart the challenge of reaching the rankings.

## How the data is stored on the Xahau blockchain

Xahau allows us to save information so that we can work with hooks and data we need. For the Ranking hook we work with 3 namespaces or tables. These tables are generated in each account that installs the hook. The two most important tables are the ADDRESS and SCORE namespaces. The namespaces in Xahau store a key and a value in each record. The key is normally used to identify the id we want to work with and the value stores the information of that id. In this case, for a ranking what we want to store is a top 5 of addresses and scores that will represent our top scoring players. As in a namespace I only store one key and one value per row, I must create two namespaces, one for addresses and one for scores.

The ADDRESS namespace or "table" would look like this:
Namespace: 19DBF7B7FC66ECCB9DDBE53345B6D28F9523488B2CE83BDED0F04B1F0D7AEFDE
| key | value|
|-----|------|
|0000000000000000000000000000000000000000000000000000000000000001 | 1E2D42546C8A5270D4E182FAE3D12186F2A32A7E |
|0000000000000000000000000000000000000000000000000000000000000002 | 1E2D42546C8A5270D4E182FAE3D12186F2A32A7E |
|0000000000000000000000000000000000000000000000000000000000000003 | 1E2D42546C8A5270D4E182FAE3D12186F2A32A7E |
|0000000000000000000000000000000000000000000000000000000000000004 | 1E2D42546C8A5270D4E182FAE3D12186F2A32A7E |
|0000000000000000000000000000000000000000000000000000000000000005 | 1E2D42546C8A5270D4E182FAE3D12186F2A32A7E |

The value 1E2D42546C8A5270D4E182FAE3D12186F2A32A7E represents the HEX translation of an XRPL address rskZVQvBEXAwsBTFsgEZMBfwnhC7oydSnp 

You can check DinoXAHur SCORE namespace on live: https://xahau.xrplwin.com/account/r4Yau7DTPk2TizjitD6p2W2hkGhorUZzn4/namespaces/7946EE7FF049DB3527BAE5E7BA6823B61570D5EA82FDEA39E227D9E223023921

The SCORE namespace or "table" would look like this:
Namespace: 48967082CCF2935B9CB27CE3335494191B6A541AF65471742285339BAD94394A
| key | value|
|-----|------|
|0000000000000000000000000000000000000000000000000000000000000001 | 2C00000000000000 |
|0000000000000000000000000000000000000000000000000000000000000002 | 0100000000000000 |
|0000000000000000000000000000000000000000000000000000000000000003 | 0100000000000000 |
|0000000000000000000000000000000000000000000000000000000000000004 | 0100000000000000 |
|0000000000000000000000000000000000000000000000000000000000000005 | 0100000000000000 |

The value 2C00000000000000 represents the HEX translation of 44 xflDecimal (uint64). 

You can check DinoXAHur SCORE namespace on live: https://xahau.xrplwin.com/account/r4Yau7DTPk2TizjitD6p2W2hkGhorUZzn4/namespaces/48967082CCF2935B9CB27CE3335494191B6A541AF65471742285339BAD94394A

## Installation & Usage

Once the hook is installed, the following triggers are expected for the hook.

- An account will send a payment of 1 XAH to the hook account. This payment if comes from an account that is not the AVOID account, will add +1 to the VAULT counter in our namespace.

- A Invoke transaction from the hook account with "SCORE" and "ADDRESS" Hook Params and values will update our Ranking in case the score is higher than any of the TOP 5 last records.

- An Invoke transaction from the hook account with the Hook Parameter "AVOID" and an Xahau address as value will add it as the account that will be avoided from the VAULT counter if it does a 1 XAH payment.

- An Invoke transaction from the hook account with the Hook Parameter "CLEAN" and 01 as value will reset the ranking top giving to every TOP 5 a score of 1 point.

- An Invoke transaction from the hook account with the Hook Parameter "RESET" and 01 as value will reset the VAULT counter to 0 deleting the key from the namespace.

## How to install the Highest Number Hook on Testnet?

HookHash: 92323532CC4281485A4B4DB4E27A1AFE81758B29C616C90546011FB9AD445268

1. You can do it by [XRPLWin Hook Install Tool](https://xahau-testnet.xrplwin.com/tools/hook/from-hash)

2. Or you can do it sending the transaction below:

HookOn is activated to trigger for Invoke and Payment transactions. You can verify it copying the HookOn value (FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFFFFFFFFFBFFFFE) in this website: https://richardah.github.io/xrpl-hookon-calculator/

    const prepared = {
      "TransactionType": "SetHook",
      "Account": your_account_address,
      "Flags": 0,
      "Hooks": [
        {
          "Hook": {
            "HookHash": "92323532CC4281485A4B4DB4E27A1AFE81758B29C616C90546011FB9AD445268",
            "HookNamespace": "0000000000000000000000000000000000000000000000000000000000000000",
            "HookOn": "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFFFFFFFFFBFFFFE",
          }
        }
      ],
      ...networkInfo.txValues,
    };

## How to install the Ranking Hook on JSHooks Testnet?

Work in progress...

## How to uninstall the Ranking Hook on Mainnet/Testnet?

    const prepared = {
      "TransactionType": "SetHook",
      "Account": your_account_address,
      "Flags": 0,
      Hooks:
    [        
        {                        
            Hook: {
                CreateCode: "",
                Flags: 1,
            }
        }
    ],
      ...networkInfo.txValues,
    };

## Transaction Examples for Hook Parameters

## Updating a new score using SCORE and ADDRESS Hook Params

We create a Invoke transaction with our Hook Account as "Account" and NO Destination Account. 

Hook Parameters and values will be:
- ADDRESS
- Xahau Address in HEX

- SCORE
- int number as uint64 LE



In this example we are using 1E2D42546C8A5270D4E182FAE3D12186F2A32A7E as address value that is the translated version of the address rskZVQvBEXAwsBTFsgEZMBfwnhC7oydSnp. You can visit https://transia-rnd.github.io/xrpl-hex-visualizer/ , Insert the account and click on From Hex button, you will see that the xrpAddress will be the same as we added. ADDRESS param string is HEX translated: 41444452455353. (For https://builder.xahau.network/ IDE you don't need to translate Parameter Name)

For SCORE value we are using 0200000000000000 as score value that is the translated version of the value 2 as uint64 LE. You can visit https://transia-rnd.github.io/xrpl-hex-visualizer/ , Insert the score value and click on From Hex button, you will see that the uint64 LE will be the same as we added. SCORE param string is HEX translated: 53434F5245. (For https://builder.xahau.network/ IDE you don't need to translate Parameter Name)

    const prepared = {
      TransactionType: "Invoke",
      Account: your_account_address,
      Flags: 0,
      HookParameters: [
        {
      "HookParameter": {
        "HookParameterName": "53434F5245",
        "HookParameterValue": "0200000000000000"
      }
    },
    {
      "HookParameter": {
        "HookParameterName": "41444452455353",
        "HookParameterValue": "1E2D42546C8A5270D4E182FAE3D12186F2A32A7E"
      }
    }
    ],
      ...networkInfo.txValues,
    };



## Using AVOID Hook Param

We create a Invoke transaction with our Hook Account as "Account" and NO Destination Account. 

Hook Parameters and values will be:
- AVOID
- Xahau Address in HEX

In this example we are using 1E2D42546C8A5270D4E182FAE3D12186F2A32A7E as address value that is the translated version of the address rskZVQvBEXAwsBTFsgEZMBfwnhC7oydSnp. You can visit https://transia-rnd.github.io/xrpl-hex-visualizer/ , Insert the account and click on From Hex button, you will see that the xrpAddress will be the same as we added. AVOID param string is HEX translated: 41564F4944. (For https://builder.xahau.network/ IDE you don't need to translate Parameter Name)


    const prepared = {
      TransactionType: "Invoke",
      Account: your_account_address,
      Flags: 0,
      HookParameters: [
        {
      "HookParameter": {
        "HookParameterName": "41564F4944",
        "HookParameterValue": "1E2D42546C8A5270D4E182FAE3D12186F2A32A7E"
      }
    }
    ],
      ...networkInfo.txValues,
    };

## Using RESET Hook Param

We create a Invoke transaction with our Hook Account as "Account" and NO Destination Account. 

Hook Parameters and values will be:
- RESET
- A number value greater than 0, for example: 01

In this example we are using 01 as value. RESET param string is HEX translated: 5245534554. (For https://builder.xahau.network/ IDE you don't need to translate Parameter Name)


    const prepared = {
      TransactionType: "Invoke",
      Account: your_account_address,
      Flags: 0,
      HookParameters: [
        {
      "HookParameter": {
        "HookParameterName": "5245534554",
        "HookParameterValue": "01"
      }
    }
    ],
      ...networkInfo.txValues,
    };

## Using CLEAN Hook Param

We create a Invoke transaction with our Hook Account as "Account" and NO Destination Account. 

Hook Parameters and values will be:
- CLEAN
- A number value greater than 0, for example: 01

In this example we are using 01 as value. CLEAN param string is HEX translated: 434C45414E. (For https://builder.xahau.network/ IDE you don't need to translate Parameter Name)


    const prepared = {
      TransactionType: "Invoke",
      Account: your_account_address,
      Flags: 0,
      HookParameters: [
        {
      "HookParameter": {
        "HookParameterName": "434C45414E",
        "HookParameterValue": "01"
      }
    }
    ],
      ...networkInfo.txValues,
    };


## How to install the Ranking Hook on Mainnet?

Same as Testnet but changing the hookhash. The Hookhash is 92323532CC4281485A4B4DB4E27A1AFE81758B29C616C90546011FB9AD445268.

1. You can do it by [XRPLWin Hook Install Tool](https://xahau.xrplwin.com/tools/hook/from-hash)

2. Or you can do it sending the transaction below:

```
    const prepared = {
      "TransactionType": "SetHook",
      "Account": your_account_address,
      "Flags": 0,
      "Hooks": [
        {
          "Hook": {
            "HookHash": "92323532CC4281485A4B4DB4E27A1AFE81758B29C616C90546011FB9AD445268",
            "HookNamespace": "0000000000000000000000000000000000000000000000000000000000000000",
            "HookOn": "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFFFFFFFFFBFFFFE",
          }
        }
      ],
      ...networkInfo.txValues,
    };
```

## I want to try them without installing anything

You can play DinoXAHur using your Xaman Wallet.


## Ideas for your hooks

This is just a small example so that others can build similar ideas or extend the current hook. Some ideas I propose:

1. The restart of the ranking is caused by a time event and not by a manual reset.
2. Create a version where some part of the vault is locked for any reasons.
3. Add a NFT mint (URI Token) with the score and the address or any type of NFT as reward.
4. Create different games that share the same ranking for any reason.

## Special thanks

Thanks to [@denis_angell](https://x.com/angell_denis) , [@Satish_nl](https://x.com/satish_nl) and [@TeQu](https://x.com/_tequ_) for being there when i get stuck.
