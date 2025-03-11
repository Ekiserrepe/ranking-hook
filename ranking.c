/**
 * A hook for the management of a ranking for a game as example. https://github.com/Ekiserrepe/ranking-hook
 */
#include "hookapi.h"

int64_t hook(uint32_t reserved ) {

    //Create reserve in case we need to send a payment
    etxn_reserve(1);


    // Check hook account
    uint8_t hook_accid[20];
    hook_account(hook_accid, 20);

    // To know the txn type
    int64_t tt = otxn_type();

    //Ranking numbers
    uint8_t one[1] = {0x01U};
    uint8_t two[1] = {0x02U};
    uint8_t three[1] = {0x03U};
    uint8_t four[1] = {0x04U};
    uint8_t five[1] = {0x05U};

    //Initial score to restart or initiate ranking
    uint64_t initial_score= 1;

    uint64_t new_ranking_score;

    uint64_t clean_score_value;

    
    //Namespace Addresses
    uint8_t addresses_ns[32] = {0x79U, 0x46U, 0xEEU, 0x7FU, 0xF0U, 0x49U, 0xDBU, 0x35U, 0x27U, 0xBAU, 0xE5U, 0xE7U, 0xBAU, 0x68U, 0x23U, 0xB6U, 0x15U, 0x70U, 0xD5U, 0xEAU, 0x82U, 0xFDU, 0xEAU, 0x39U, 0xE2U, 0x27U, 0xD9U, 0xE2U, 0x23U, 0x02U, 0x39U, 0x21U};    
    uint8_t scores_ns[32] = { 0x48U, 0x96U, 0x70U, 0x82U, 0xCCU, 0xF2U, 0x93U, 0x5BU, 0x9CU, 0xB2U, 0x7CU, 0xE3U, 0x33U, 0x54U, 0x94U, 0x19U, 0x1BU, 0x6AU, 0x54U, 0x1AU, 0xF6U, 0x54U, 0x71U, 0x74U, 0x22U, 0x85U, 0x33U, 0x9BU, 0xADU, 0x94U, 0x39U, 0x4AU };    


    //Get score from namespace
    uint64_t get_score_one;
    uint64_t get_score_two;
    uint64_t get_score_three;
    uint64_t get_score_four;
    uint64_t get_score_five;

    //Get actual score from ranking namespace
    state_foreign(SVAR(get_score_one), SBUF(one), SBUF(scores_ns), SBUF(hook_accid));
    state_foreign(SVAR(get_score_two), SBUF(two), SBUF(scores_ns), SBUF(hook_accid));
    state_foreign(SVAR(get_score_three), SBUF(three), SBUF(scores_ns), SBUF(hook_accid));
    state_foreign(SVAR(get_score_four), SBUF(four), SBUF(scores_ns), SBUF(hook_accid));
    state_foreign(SVAR(get_score_five), SBUF(five), SBUF(scores_ns), SBUF(hook_accid));

    uint8_t get_address_1[20];
    uint8_t get_address_2[20];
    uint8_t get_address_3[20];
    uint8_t get_address_4[20];
    uint8_t get_address_5[20];
    //Get actual addresses from ranking namespace
    state_foreign(SVAR(get_address_1), SBUF(one), SBUF(addresses_ns), SBUF(hook_accid));
    state_foreign(SVAR(get_address_2), SBUF(two), SBUF(addresses_ns), SBUF(hook_accid));
    state_foreign(SVAR(get_address_3), SBUF(three), SBUF(addresses_ns), SBUF(hook_accid));
    state_foreign(SVAR(get_address_4), SBUF(four), SBUF(addresses_ns), SBUF(hook_accid));
    state_foreign(SVAR(get_address_5), SBUF(five), SBUF(addresses_ns), SBUF(hook_accid));

    // Check destination of the original txn
    uint8_t account_field[20];
    int32_t account_field_len = otxn_field(SBUF(account_field), sfDestination);

    // Get the account that is sending the amount and broke the record.
    uint8_t account_sender[20];
    int32_t sender_field_len = otxn_field(SBUF(account_sender), sfAccount);



    uint16_t score[32];

    //Old account records addresses
    uint8_t account01[20] = {0x00U};
    uint8_t account02[20] = {0x00U};
    uint8_t account03[20] = {0x00U};
    uint8_t account04[20] = {0x00U};
    uint8_t account05[20] = {0x00U};

    //Avoid account from param, the account that will not be debited if sending payments
    uint8_t avoid_account_param[20] = {0x00U};

    //New account for the ranking in case it surpasses the ranking
    uint8_t new_ranking_address[20] = {0x00U};

    //Hook params
    uint8_t avoid_param[5] = {'A', 'V', 'O', 'I','D'};
    uint8_t reset_param[5] = {'R', 'E', 'S', 'E','T'};
    uint8_t rank_address_param[7] = {'A', 'D', 'D', 'R','E','S','S'};
    uint8_t rank_score_param[5] = {'S', 'C', 'O', 'R','E'};
    uint8_t vault_param[5] = {'V', 'A', 'U', 'L','T'};
    uint8_t cleanscore_param[5] = {'C', 'L', 'E', 'A','N'};


    //Avoid account from namespace, the account that will not be debited in our vault if we are sending payments
    uint8_t avoid_account_ns[20] = {0x00U};
    //Check if there is an avoid account
    state(SVAR(avoid_account_ns), avoid_param, 5);
    // Checking if its avoid account who is sending the payment
    int equal_avoid = 0;
    BUFFER_EQUAL(equal_avoid, avoid_account_ns, account_sender, 20);
    
    // Checking if hook_accid and account_field are the same
    int equal = 0;
    BUFFER_EQUAL(equal, hook_accid, account_field, 20);

    //Get the avoid account Avoid account, the account that will not be debited if sending payments
    otxn_param(SVAR(avoid_account_param), SBUF(avoid_param));

    uint64_t vault_amount_ns = 0;
    state(SVAR(vault_amount_ns), vault_param, 5);

    //Checking type of Amount (XAH or IOUs)
    unsigned char amount_buffer[48];
    int64_t amount_len = otxn_field(SBUF(amount_buffer), sfAmount);
    int64_t drops_sent = 0; // this will be the default
        if (tt==0 && amount_len != 8){
        rollback(SBUF("Ranking: Not accepting IOUs or transaction type."), 3);
        }
    else // If it's XAH
    {
        int64_t otxn_drops = AMOUNT_TO_DROPS(amount_buffer);    
        drops_sent = (int64_t)((double)otxn_drops);
    }

    uint8_t ns_length[20];
    //Generate initial ranking if there is no ranking
  if(state_foreign(SVAR(ns_length), SVAR(one), SBUF(addresses_ns), SBUF(hook_accid))!=20){
        state_foreign_set(SVAR(hook_accid), SBUF(one), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(hook_accid), SBUF(two), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(hook_accid), SBUF(three), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(hook_accid), SBUF(four), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(hook_accid), SBUF(five), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(one), SBUF(scores_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(two), SBUF(scores_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(three), SBUF(scores_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(four), SBUF(scores_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(five), SBUF(scores_ns), SBUF(hook_accid));
    }

    if(tt==99 && otxn_param(SVAR(clean_score_value), SBUF(cleanscore_param))>0){
        state_foreign_set(SVAR(hook_accid), SBUF(one), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(hook_accid), SBUF(two), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(hook_accid), SBUF(three), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(hook_accid), SBUF(four), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(hook_accid), SBUF(five), SBUF(addresses_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(one), SBUF(scores_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(two), SBUF(scores_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(three), SBUF(scores_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(four), SBUF(scores_ns), SBUF(hook_accid));
        state_foreign_set(SVAR(initial_score), SBUF(five), SBUF(scores_ns), SBUF(hook_accid));
        accept(SBUF("Ranking: Reset Ranking Scores."), 1);
    }

    // If i want to write or update the avoid account in the namesapace, I need an invoke from the hooks account
    if (!equal && tt == 99 && otxn_param(avoid_account_param, 20, SBUF(avoid_param))==20){
        state_set(SVAR(avoid_account_param), avoid_param, 5);
        accept(SBUF("Ranking: Writing Avoid Account."), 2);
    }

    // If i want to reset the vault amount. Invoke from hook account (!equal),
    uint64_t reset_value;
    if (!equal && tt == 99 && otxn_param(SVAR(reset_value), SBUF(reset_param))>0){
        state_set(0,0, vault_param,5);
        accept(SBUF("Ranking: Reseting Amount Vault."), 3);
    }

    //If there is a payment of 1 XAH, we add it to the vault (It's not avoid_account !equal_avoid, It's a incoming payment, equal, it's a payment tt==0 and 1 XAH)
    if(!equal_avoid && equal && tt==0 && drops_sent==1000000){
        vault_amount_ns += 1;
        state_set(SVAR(vault_amount_ns), vault_param, 5);
        accept(SBUF("Ranking: Adding +1 to the vault."), 4);
    }
    
    //If the new limit is bigger I record the new limit
    //Invoke with ADDRESS param and SCORE param
    if(!equal && tt==99 && otxn_param(new_ranking_address, 20, SBUF(rank_address_param))==20 && otxn_param(SVAR(new_ranking_score), SBUF(rank_score_param))>0){

        //1st Ranking Rewritting
        if(new_ranking_score>get_score_one){
            uint64_t actual_vault;
            state(SVAR(actual_vault), vault_param, 5);
            uint64_t drops_to_send = (int64_t)((double)actual_vault);
            uint64_t vault_drops=drops_to_send*1000000;            
            unsigned char tx01[PREPARE_PAYMENT_SIMPLE_SIZE];
            PREPARE_PAYMENT_SIMPLE(tx01, vault_drops, new_ranking_address, 0, 0);
            uint8_t emithash01[32];
            int64_t emit_result01 = emit(SBUF(emithash01), SBUF(tx01));
            state_set(0,0, vault_param, 5);
            state_foreign_set(SVAR(new_ranking_score), SBUF(one), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_one), SBUF(two), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_two), SBUF(three), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_three), SBUF(four), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_four), SBUF(five), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(new_ranking_address), SBUF(one), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_1), SBUF(two), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_2), SBUF(three), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_3), SBUF(four), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_4), SBUF(five), SBUF(addresses_ns), SBUF(hook_accid));
            accept(SBUF("Ranking: New 1st Ranking position. Vault sent in case there is any"), 5);
        } else if (new_ranking_score>get_score_two) {
            state_foreign_set(SVAR(new_ranking_score), SBUF(two), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_two), SBUF(three), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_three), SBUF(four), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_four), SBUF(five), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(new_ranking_address), SBUF(two), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_2), SBUF(three), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_3), SBUF(four), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_4), SBUF(five), SBUF(addresses_ns), SBUF(hook_accid));
            accept(SBUF("Ranking: New 2nd Rakning."), 6);
        } else if (new_ranking_score>get_score_three) {
            state_foreign_set(SVAR(new_ranking_score), SBUF(three), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_three), SBUF(four), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_four), SBUF(five), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(new_ranking_address), SBUF(three), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_3), SBUF(four), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_4), SBUF(five), SBUF(addresses_ns), SBUF(hook_accid));
            accept(SBUF("Ranking: New 3rd Ranking position."), 7);
        } else if (new_ranking_score>get_score_four) {
            state_foreign_set(SVAR(new_ranking_score), SBUF(four), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_score_four), SBUF(five), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(new_ranking_address), SBUF(four), SBUF(addresses_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(get_address_4), SBUF(five), SBUF(addresses_ns), SBUF(hook_accid));
            accept(SBUF("Ranking: New 4th Ranking position."), 8);
        } else if (new_ranking_score>get_score_five) {
            state_foreign_set(SVAR(new_ranking_score), SBUF(five), SBUF(scores_ns), SBUF(hook_accid));
            state_foreign_set(SVAR(new_ranking_address), SBUF(five), SBUF(addresses_ns), SBUF(hook_accid));
            accept(SBUF("Ranking: New 5th Ranking position."), 9);
        }
        
    }
    accept (0,0,0); 
    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}