<?php
/**
 * @file
 * storyhost_renewal.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function storyhost_renewal_default_rules_configuration() {
  $items = array();
  $items['rules_sh_general_expire'] = entity_import('rules_config', '{ "rules_sh_general_expire" : {
      "LABEL" : "SH Renewal - Expire Storyhost",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "SH Renewal" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "9" : "9" } } } },
        { "OR" : [
            { "data_is" : {
                "data" : [ "user:field-took-ash1" ],
                "op" : "\\u003C",
                "value" : "-1 years"
              }
            },
            { "data_is" : {
                "data" : [ "user:field-took-ash2" ],
                "op" : "\\u003C",
                "value" : "-1 years"
              }
            }
          ]
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "user:field-powers-expire" ], "value" : "now" } },
        { "user_remove_role" : { "account" : [ "user" ], "roles" : { "value" : { "9" : "9" } } } },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "15" : "15" } } } },
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] Your SH privileges have expired",
            "message" : "You are receiving this email because it has been at least a year since you passed ASH Test One and ASH Test Two.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that SH\\u0027s retake these tests regularly so to keep your skills sharp and consistent with the rest of the team. Over time personal style in ruling can deviate and the longer the time the more so, hence the refresher.\\r\\n\\r\\nAs soon as you retake both ASH Test One and ASH Test Two you will be re-enabled automatically as a SH.\\r\\n\\r\\nASH Test 1: http:\\/\\/vaxia.org\\/content\\/ash-test-1-story-hosting-basics\\r\\nASH Test 2: http:\\/\\/vaxia.org\\/content\\/ash-test-2-renewing-shs\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "14" : "14", "21" : "21", "3" : "3" } },
            "subject" : "[Vaxia] A storyhosts privileges have expired",
            "message" : "[user:name] is up for SH renewal because it has been at least a year since they passed ASH Test One and ASH Test Two.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that SH\\u0027s retake these tests regularly so to keep skills sharp and consistent with the rest of the team. Over time personal style in ruling can deviate, and the longer the time the more so, hence the refresher.\\r\\n\\r\\nAs soon as [user:name] retakes both ASH Test One and ASH Test Two [user:name] will be re-enabled automatically as a SH.\\r\\n\\r\\nASH Test 1: http:\\/\\/vaxia.org\\/content\\/ash-test-1-story-hosting-basics\\r\\nASH Test 2: http:\\/\\/vaxia.org\\/content\\/ash-test-2-renewing-shs\\r\\n\\r\\nThis email is just a courtesy to let you know about the temporary change in status.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~"
          }
        }
      ]
    }
  }');
  $items['rules_sh_general_expire_warning'] = entity_import('rules_config', '{ "rules_sh_general_expire_warning" : {
      "LABEL" : "SH Renewal - Expire Storyhost Warning",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "SH Renewal" ],
      "REQUIRES" : [ "rules", "php", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "9" : "9" } } } },
        { "OR" : [
            { "data_is" : {
                "data" : [ "user:field-took-ash1" ],
                "value" : {
                  "select" : "site:current-date",
                  "date_offset" : { "value" : -28512000 }
                }
              }
            },
            { "data_is" : {
                "data" : [ "user:field-took-ash2" ],
                "value" : {
                  "select" : "site:current-date",
                  "date_offset" : { "value" : -28512000 }
                }
              }
            }
          ]
        },
        { "NOT user_is_blocked" : { "account" : [ "user" ] } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] Your SH privileges are up for renewal",
            "message" : "You are receiving this email because it has almost been a year since you passed ASH Test One and ASH Test Two.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that SH\\u0027s retake these tests regularly so to keep your skills sharp and consistent with the rest of the team. Over time personal style in ruling can deviate, and the longer the time the more so. We ask that you take a refresher every year.\\r\\n\\r\\nPlease be sure to retake these tests before \\u003C?php print date(\\u0027m\\/d\\/Y\\u0027, strtotime(\\u0027+1 month\\u0027)); ?\\u003E to avoid losing your permissions.\\r\\n\\r\\nASH Test 1: http:\\/\\/vaxia.org\\/content\\/ash-test-1-story-hosting-basics\\r\\nASH Test 2: http:\\/\\/vaxia.org\\/content\\/ash-test-2-renewing-shs\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "4" : "4" } },
            "subject" : "[Vaxia] A SH privileges are up for renewal",
            "message" : "[user:name] is up for SH renewal because it has almost been a year since they passed ASH Test One and ASH Test Two.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that SH\\u0027s retake these tests regularly so to keep skills sharp and consistent with the rest of the team.\\r\\n\\r\\n[user:name] needs to retake the ASH tests before \\u003C?php print date(\\u0027m\\/d\\/Y\\u0027, strtotime(\\u0027+1 month\\u0027)); ?\\u003E to avoid losing their permissions.\\r\\n\\r\\nASH Test 1: http:\\/\\/vaxia.org\\/content\\/ash-test-1-story-hosting-basics\\r\\nASH Test 2: http:\\/\\/vaxia.org\\/content\\/ash-test-2-renewing-shs\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~"
          }
        }
      ]
    }
  }');
  $items['rules_sh_renewal'] = entity_import('rules_config', '{ "rules_sh_renewal" : {
      "LABEL" : "SH Renewal - Renew Storyhost",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "1",
      "OWNER" : "rules",
      "TAGS" : [ "SH Renewal" ],
      "REQUIRES" : [ "rules", "quiz_rules" ],
      "ON" : { "quiz_rules_quiz_passed" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "15" : "15", "9" : "9" } },
            "operation" : "OR"
          }
        },
        { "data_is" : {
            "data" : [ "user:field-took-ash1" ],
            "op" : "\\u003E",
            "value" : "-1 month"
          }
        },
        { "data_is" : {
            "data" : [ "user:field-took-ash2" ],
            "op" : "\\u003E",
            "value" : "-1 month"
          }
        },
        { "OR" : [
            { "data_is" : { "data" : [ "node:nid" ], "value" : "7037" } },
            { "data_is" : { "data" : [ "node:nid" ], "value" : "7781" } }
          ]
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "user:field-powers-expire" ], "value" : "+3 months" } },
        { "user_remove_role" : { "account" : [ "user" ], "roles" : { "value" : { "15" : "15" } } } },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "9" : "9" } } } },
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] You have renewed your SH permissions",
            "message" : "You have renewed their Storyhost permissions by passing ASH Test 1 and ASH Test 2 within the last month.\\r\\n\\r\\nCongratulations!\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9" } },
            "subject" : "[Vaxia] A SH has renewed permissions",
            "message" : "[user:name] has renewed their Storyhost permissions by passing ASH Test 1 and ASH Test 2 - Renewal within the last month.\\r\\n\\r\\nRenewed Storyhost: [user:name]\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~"
          }
        }
      ]
    }
  }');
  $items['sh_test_evaluator_renewal'] = entity_import('rules_config', '{ "sh_test_evaluator_renewal" : {
      "LABEL" : "SH Renewal - Renew Evaluator",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "1",
      "OWNER" : "rules",
      "TAGS" : [ "SH Renewal" ],
      "REQUIRES" : [ "rules", "quiz_rules" ],
      "ON" : { "quiz_rules_quiz_passed" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "20" : "20", "14" : "14" } },
            "operation" : "OR"
          }
        },
        { "data_is" : { "data" : [ "node:nid" ], "value" : "9359" } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "user:field-powers-expire" ], "value" : "+6 months" } },
        { "user_remove_role" : { "account" : [ "user" ], "roles" : { "value" : { "20" : "20" } } } },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "14" : "14" } } } },
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] You have renewed your Evaluator permissions",
            "message" : "You have renewed you Evaluator permissions by passing the Evaluator Test within the last month.\\r\\n\\r\\nCongratulations!\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "14" : "14" } },
            "subject" : "[Vaxia] An Evaluator has renewed permissions",
            "message" : "[user:name] has renewed their Evaluator permissions by passing the Evaluator Test within the last month.\\r\\n\\r\\nRenewed Evaluator: [user:name]\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~"
          }
        }
      ]
    }
  }');
  $items['vaxia_accounts_sh_eval_expire'] = entity_import('rules_config', '{ "vaxia_accounts_sh_eval_expire" : {
      "LABEL" : "SH Renewal - Expire Evaluator",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "SH Renewal" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "14" : "14" } } } },
        { "data_is" : {
            "data" : [ "user:field-took-eval-test" ],
            "op" : "\\u003C",
            "value" : "-1 year"
          }
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "user:field-powers-expire" ], "value" : "+3 months" } },
        { "user_remove_role" : { "account" : [ "user" ], "roles" : { "value" : { "14" : "14" } } } },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "20" : "20" } } } },
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] Your evaluator privileges have expired",
            "message" : "You are receiving this email because it has been at least a year since you passed ASH Test Four \\/ Eval Test.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that evaluators retake these tests regularly so to keep your skills sharp and consistent with the rest of the team. Over time personal style in ruling can deviate and the longer the time the more so, hence the refresher.\\r\\n\\r\\nAs soon as you retake ASH Test Four \\/ Eval Test you will be re-enabled automatically as a Evaluator.\\r\\n\\r\\nEvaluator Test: http:\\/\\/vaxia.org\\/content\\/ash-test-4-item-and-character-evaluation\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "14" : "14", "21" : "21", "3" : "3" } },
            "subject" : "[Vaxia] An evaluators privileges have expired",
            "message" : "[user:name] is up for Evaluator renewal because it has been at least a year since they passed ASH Test Four.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that SH\\u0027s retake these tests regularly so to keep skills sharp and consistent with the rest of the team. Over time personal style in ruling can deviate, and the longer the time the more so, hence the refresher.\\r\\n\\r\\nAs soon as [user:name] retakes ASH Test Four [user:name] will be re-enabled automatically as an Evaluator.\\r\\n\\r\\nASH Test 4: http:\\/\\/vaxia.org\\/content\\/ash-test-4-item-and-character-evaluation\\r\\n\\r\\nThis email is just a courtesy to let you know about the temporary change in status.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~"
          }
        }
      ]
    }
  }');
  $items['vaxia_accounts_sh_eval_expire_warning'] = entity_import('rules_config', '{ "vaxia_accounts_sh_eval_expire_warning" : {
      "LABEL" : "SH Renewal - Expire Evaluator Warning",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "SH Renewal" ],
      "REQUIRES" : [ "rules", "php", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "14" : "14" } } } },
        { "data_is" : { "data" : [ "user:field-took-eval-test" ], "value" : "-11 months" } },
        { "NOT user_is_blocked" : { "account" : [ "user" ] } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] Your Evaluator privileges are up for renewal",
            "message" : "You are receiving this email because it has almost been a year since you passed ASH Test Four \\/ Eval Test.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that Evaluators retake these tests regularly so to keep your skills sharp and consistent with the rest of the team. Over time personal style in ruling can deviate, and the longer the time the more so. We ask that you take a refresher every year.\\r\\n\\r\\nPlease be sure to retake these tests before \\u003C?php print date(\\u0027m\\/d\\/Y\\u0027, strtotime(\\u0027+1 month\\u0027)); ?\\u003E to avoid losing your permissions.\\r\\n\\r\\nEvaluator Test: http:\\/\\/vaxia.org\\/content\\/ash-test-4-item-and-character-evaluation\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "14" : "14", "4" : "4" } },
            "subject" : "[Vaxia] An Evaluators privileges are up for renewal",
            "message" : "[user:name] is up for Evaluator renewal because it has almost been a year since they passed ASH Test Four \\/ Eval Test.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that Evaluators retake these tests regularly so to keep skills sharp and consistent with the rest of the team.\\r\\n\\r\\n[user:name] needs to retake the Evaluator test before \\u003C?php print date(\\u0027m\\/d\\/Y\\u0027, strtotime(\\u0027+1 month\\u0027)); ?\\u003E to avoid losing their permissions.\\r\\n\\r\\nEvaluator Test : http:\\/\\/vaxia.org\\/content\\/ash-test-4-item-and-character-evaluation\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~"
          }
        }
      ]
    }
  }');
  return $items;
}