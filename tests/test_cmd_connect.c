#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "xmpp/xmpp.h"

#include "ui/ui.h"
#include "ui/stub_ui.h"

#include "command/commands.h"
#include "config/accounts.h"

static void test_with_connection_status(jabber_conn_status_t status)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));

    will_return(jabber_get_connection_status, status);

    expect_cons_show("You are either connected already, or a login is in process.");

    gboolean result = cmd_connect(NULL, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_message_when_disconnecting(void **state)
{
    test_with_connection_status(JABBER_DISCONNECTING);
}

void cmd_connect_shows_message_when_connecting(void **state)
{
    test_with_connection_status(JABBER_CONNECTING);
}

void cmd_connect_shows_message_when_connected(void **state)
{
    test_with_connection_status(JABBER_CONNECTED);
}

void cmd_connect_shows_message_when_undefined(void **state)
{
    test_with_connection_status(JABBER_UNDEFINED);
}

void cmd_connect_shows_usage_when_no_server_value(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    help->usage = "some usage";
    gchar *args[] = { "user@server.org", "server", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Usage: some usage");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_usage_when_server_no_port_value(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    help->usage = "some usage";
    gchar *args[] = { "user@server.org", "server", "aserver", "port", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Usage: some usage");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_usage_when_no_port_value(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    help->usage = "some usage";
    gchar *args[] = { "user@server.org", "port", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Usage: some usage");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_usage_when_port_no_server_value(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    help->usage = "some usage";
    gchar *args[] = { "user@server.org", "port", "5678", "server", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Usage: some usage");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_message_when_port_0(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "user@server.org", "port", "0", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Value 0 out of range. Must be in 1..65535.");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_message_when_port_minus1(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "user@server.org", "port", "-1", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Value -1 out of range. Must be in 1..65535.");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_message_when_port_65536(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "user@server.org", "port", "65536", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Value 65536 out of range. Must be in 1..65535.");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_message_when_port_contains_chars(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "user@server.org", "port", "52f66", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Could not convert \"52f66\" to a number.");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_usage_when_server_provided_twice(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    help->usage = "some usage";
    gchar *args[] = { "user@server.org", "server", "server1", "server", "server2", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Usage: some usage");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_usage_when_port_provided_twice(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    help->usage = "some usage";
    gchar *args[] = { "user@server.org", "port", "1111", "port", "1111", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Usage: some usage");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_usage_when_invalid_first_property(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    help->usage = "some usage";
    gchar *args[] = { "user@server.org", "wrong", "server", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Usage: some usage");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_usage_when_invalid_second_property(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    help->usage = "some usage";
    gchar *args[] = { "user@server.org", "server", "aserver", "wrong", "1234", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_cons_show("Usage: some usage");
    expect_cons_show("");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_when_no_account(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "user@server.org", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_string(accounts_get_account, name, "user@server.org");
    will_return(accounts_get_account, NULL);

    will_return(ui_ask_password, strdup("password"));

    expect_cons_show("Connecting as user@server.org");

    expect_string(jabber_connect_with_details, jid, "user@server.org");
    expect_string(jabber_connect_with_details, passwd, "password");
    expect_value(jabber_connect_with_details, altdomain, NULL);
    expect_value(jabber_connect_with_details, port, 0);
    will_return(jabber_connect_with_details, JABBER_CONNECTING);

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_with_server_when_provided(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "user@server.org", "server", "aserver", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_string(accounts_get_account, name, "user@server.org");
    will_return(accounts_get_account, NULL);

    will_return(ui_ask_password, strdup("password"));

    expect_cons_show("Connecting as user@server.org");

    expect_string(jabber_connect_with_details, jid, "user@server.org");
    expect_string(jabber_connect_with_details, passwd, "password");
    expect_string(jabber_connect_with_details, altdomain, "aserver");
    expect_value(jabber_connect_with_details, port, 0);
    will_return(jabber_connect_with_details, JABBER_CONNECTING);

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_with_port_when_provided(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "user@server.org", "port", "5432", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_string(accounts_get_account, name, "user@server.org");
    will_return(accounts_get_account, NULL);

    will_return(ui_ask_password, strdup("password"));

    expect_cons_show("Connecting as user@server.org");

    expect_string(jabber_connect_with_details, jid, "user@server.org");
    expect_string(jabber_connect_with_details, passwd, "password");
    expect_value(jabber_connect_with_details, altdomain, NULL);
    expect_value(jabber_connect_with_details, port, 5432);
    will_return(jabber_connect_with_details, JABBER_CONNECTING);

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_with_server_and_port_when_provided(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "user@server.org", "port", "5432", "server", "aserver", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_string(accounts_get_account, name, "user@server.org");
    will_return(accounts_get_account, NULL);

    will_return(ui_ask_password, strdup("password"));

    expect_cons_show("Connecting as user@server.org");

    expect_string(jabber_connect_with_details, jid, "user@server.org");
    expect_string(jabber_connect_with_details, passwd, "password");
    expect_string(jabber_connect_with_details, altdomain, "aserver");
    expect_value(jabber_connect_with_details, port, 5432);
    will_return(jabber_connect_with_details, JABBER_CONNECTING);

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_fail_message(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "user@server.org", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_any(accounts_get_account, name);
    will_return(accounts_get_account, NULL);

    will_return(ui_ask_password, strdup("password"));

    expect_cons_show("Connecting as user@server.org");

    expect_any(jabber_connect_with_details, jid);
    expect_any(jabber_connect_with_details, passwd);
    expect_any(jabber_connect_with_details, altdomain);
    expect_any(jabber_connect_with_details, port);
    will_return(jabber_connect_with_details, JABBER_DISCONNECTED);

    expect_cons_show_error("Connection attempt for user@server.org failed.");

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_lowercases_argument(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "USER@server.ORG", NULL };

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_string(accounts_get_account, name, "user@server.org");
    will_return(accounts_get_account, NULL);

    will_return(ui_ask_password, strdup("password"));

    expect_cons_show("Connecting as user@server.org");

    expect_any(jabber_connect_with_details, jid);
    expect_any(jabber_connect_with_details, passwd);
    expect_any(jabber_connect_with_details, altdomain);
    expect_any(jabber_connect_with_details, port);
    will_return(jabber_connect_with_details, JABBER_CONNECTING);

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_asks_password_when_not_in_account(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "jabber_org", NULL };
    ProfAccount *account = account_new("jabber_org", "me@jabber.org", NULL, NULL,
        TRUE, NULL, 0, NULL, NULL, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL);

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_any(accounts_get_account, name);
    will_return(accounts_get_account, account);

    will_return(ui_ask_password, strdup("password"));

    expect_cons_show("Connecting with account jabber_org as me@jabber.org");

    expect_any(jabber_connect_with_account, account);
    will_return(jabber_connect_with_account, JABBER_CONNECTING);

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_shows_message_when_connecting_with_account(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "jabber_org", NULL };
    ProfAccount *account = account_new("jabber_org", "user@jabber.org", "password", NULL,
        TRUE, NULL, 0, "laptop", NULL, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL);

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_any(accounts_get_account, name);
    will_return(accounts_get_account, account);

    expect_cons_show("Connecting with account jabber_org as user@jabber.org/laptop");

    expect_any(jabber_connect_with_account, account);
    will_return(jabber_connect_with_account, JABBER_CONNECTING);

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}

void cmd_connect_connects_with_account(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));
    gchar *args[] = { "jabber_org", NULL };
    ProfAccount *account = account_new("jabber_org", "me@jabber.org", "password", NULL,
        TRUE, NULL, 0, NULL, NULL, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL);

    will_return(jabber_get_connection_status, JABBER_DISCONNECTED);

    expect_any(accounts_get_account, name);
    will_return(accounts_get_account, account);

    expect_cons_show("Connecting with account jabber_org as me@jabber.org");

    expect_memory(jabber_connect_with_account, account, account, sizeof(account));
    will_return(jabber_connect_with_account, JABBER_CONNECTING);

    gboolean result = cmd_connect(args, *help);
    assert_true(result);

    free(help);
}
