#!/usr/bin/perl
#
# accounts.pl - parses the account information file and outputs a form which displays
#               the list of email accounts.

print "Content-type: text/html\r\n\r\n";
use strict;

# global variables
#
my ($blocked) = 0;
my ($query)   = $ENV{'QUERY_STRING'};
my ($header_html) = "Accounts_H";

my (@usernames,@passwords,@addresses,@flags, @localpophostnames, @localsmtphostnames);
my ($record_number, $global_pop_hostname, $global_smtp_hostname);
my ($edithosts);

my ($public_path, $accounts_file);

# main
#

if (!get_public_path())
{
  output_header_html($header_html);
  output_buttons(1);
  output_error(3);
}
else
{
  $accounts_file = $public_path."/NCMail/Users";

  # **** Only output the buttons and main part of the page if the file could be found
  if (!check_file_exists($accounts_file))
  {
    output_header_html($header_html);
    output_buttons(1);
    output_error(1);
  }
  else
  {
    read_accounts_file($accounts_file);

    # **** Check that the accounts haven't been blocked
    if ($blocked == 1)
    {
      output_header_html($header_html);
      output_buttons(1);
      output_error(2);
    }
    else
    {
      output_header_html($header_html);
      output_buttons(2);
      output_main_html();
    }
  }
}


# read the accounts file
#
sub read_accounts_file
{
  my ($file) = @_;
  my ($line, @fields, $i);

  if (!open(FILE,$file))
  {
    print("Error opening $file : $!\n");
    return;
  }

  @usernames = ("");
  @passwords = ("");
  @addresses = ("");
  @flags = ("");
  @localpophostnames = ("");
  @localsmtphostnames = ("");
  $record_number = 0;
  $edithosts = 0;

  # **** Search through the users file for the records
  while ($line = <FILE>)
  {
    @fields = split(/ /,$line);

    if ($fields[0] eq "POP3hostname")
    {
      $global_pop_hostname = $fields[1];
    }

    if ($fields[0] eq "SMTPhostname")
    {
      $global_smtp_hostname = $fields[1];
    }

    if ($fields[0] eq "EditHosts")
    {
      $edithosts = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "Blocked")
    {
      $blocked = $fields[1];
    }

    if ($fields[0] eq "pop3username")
    {
      $usernames[$record_number] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "pop3password")
    {
      $passwords[$record_number] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "flags")
    {
      $flags[$record_number] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "localpop3")
    {
      $localpophostnames[$record_number] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "localsmtp")
    {
      $localsmtphostnames[$record_number] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "emailaddress")
    {
      $addresses[$record_number] = substr($fields[1], 0, length($fields[1]) - 1);

      # increment the record_number when the email address is found as it is the last part of each account record
      $record_number += 1;
    }
  }

  close (FILE);

  if ($usernames[0] eq "") # Nothing in the file
  {
    $blocked = 1;
  }

  # Check that the fields are all filled, even if just the default settings
  for ($i = 0; $i < $record_number; $i++)
  {
    if ($localpophostnames[$i] eq "" or $localpophostnames[$i] eq "-") { $localpophostnames[$i] = $global_pop_hostname; }
    if ($localsmtphostnames[$i] eq "" or $localsmtphostnames[$i] eq "-") { $localsmtphostnames[$i] = $global_smtp_hostname; }
  }

  return 1;
}


# output_header_html
#
sub output_header_html
{
  my ($file) = @_;
  my ($line);

  # **** Output the header of the page, with the buttons
  if (!open(FILE,$file))
  {
      print("Error opening $file : $!\n");
      return;
  }

  # **** read the file in
  while ($line = <FILE>)
  {
      print("$line");
  }

  close (FILE);
}


# output_main_html
#
sub output_main_html
{
  my ($count, $i, $default);

  # **** Output the start of the table
  print "<BR><BR><BR>\n<CENTER>\n\n";
  print "<TABLE BORDER=0 WIDTH=548>\n";
  print "<FONT FACE=ncoffline SIZE=êêglobal_fontsize_lgëë><TR><TD>êêaccounts_addressëë</TD><TD></TD><TD ALIGN=CENTER>êêaccounts_defaultëë</TD></TR>\n<TR><TD COLSPAN=3><HR></TD></TR></FONT>";

  # **** Output the names read from the users file
  $count = @usernames;

  for ($i = 0; $i < $count; $i++)
  {
    if ($flags[$i] & 1)
    {
      $default = "<IMG SRC=\"file:/NCMailUIRes:pics/tick.gif\">";
    }
    else
    {
      $default = "";
    }

    print "<TR>\n";
    print "  <TD><FONT FACE=ncoffline SIZE=êêglobal_fontsize_lgëë><A HREF=\"modify.pl?path=".$public_path."&user=".$i."\">";

    # print $usernames[$i]."@".$localpophostnames[$i]; WRONG!
    print $addresses[$i];

    # print "  </A></FONT></TD><TD WIDTH=5></TD><TD ALIGN=CENTER VALIGN=CENTER><FONT FACE=ncoffline SIZE=êêglobal_fontsize_lgëë COLOR=GREEN>".$default."</FONT></TD>\n</TR>\n";
    print "  </A></FONT></TD><TD WIDTH=5></TD><TD ALIGN=CENTER VALIGN=CENTER>".$default."</TD>\n</TR>\n";

    # print "Password (".$i.") - ".$passwords[$i]."\n";
    # print "Email (".$i.") - ".$addresses[$i]."\n";
  }

  # **** Output the end of the table
  print "</TABLE>\n\n";

  print "</BODY>\n";
  print "</HTML>\n";
}


# check_file_exists
#
sub check_file_exists
{
  my ($file) = @_;

  if (!open(FILE,$file))
  {
    return 0;
  }

  return 1;
}


# output_error
#
sub output_error
{
  my ($error_type) = @_;

  # **** Output the error message
  print "<BR><BR><BR>\n<CENTER>\n\n";

  if ($error_type == 1)
  {
    print "<FONT FACE=ncoffline SIZE=êêglobal_fontsize_lgëë>êêaccounts_error01ëë</FONT>\n";
  }

  if ($error_type == 2)
  {
    print "<FONT FACE=ncoffline SIZE=êêglobal_fontsize_lgëë>êêaccounts_error02ëë</FONT>\n";
  }

  if ($error_type == 3)
  {
    print "<FONT FACE=ncoffline SIZE=êêglobal_fontsize_lgëë>êêaccounts_error03ëë</FONT>\n";
  }

}


# output_buttons
#
sub output_buttons
{
  my ($number_of_buttons) = @_;

  print "<TABLE WIDTH=\"100%\" ALIGN=CENTER BORDER=0 CELLSPACING=0>";
  print "  <TR>";

  if ($number_of_buttons == 2)
  {
    print "    <TD ALIGN=CENTER>";
    print "      <FONT FACE=ncoffline SIZE=êêglobal_fontsize_lgëë>";
    print "      <FORM METHOD=\"GET\" ACTION=\"javascript:return_to_usersel()\" TARGET=\"_top\">";
    print "        <INPUT TYPE=SUBMIT VALUE=\"êêaccounts_b_inboxëë\" BORDERIMAGE=\"icontype:buttmid_bord\" SELIMAGE=\"icontype:buttmid_sel\" WIDTH=110 HEIGHT=40>";
    print "      </FORM>";
    print "      </FONT>";
    print "    </TD>";
  }

  print "    <TD ALIGN=CENTER>";
  print "      <FONT FACE=ncoffline SIZE=êêglobal_fontsize_lgëë>";
  print "      <FORM METHOD=\"GET\" ACTION=\"create.pl\" TARGET=\"_top\">";
  print "        <INPUT TYPE=HIDDEN NAME=\"path\" VALUE=".$public_path.">";
  print "        <INPUT TYPE=SUBMIT VALUE=\"êêaccounts_b_createëë\" BORDERIMAGE=\"icontype:buttmid_bord\" SELIMAGE=\"icontype:buttmid_sel\" WIDTH=110 HEIGHT=40>";
  print "      </FORM>";
  print "      </FONT>";
  print "    </TD>";
  print "  </TR>";
  print "</TR>";
  print "</TABLE>";

}


# get_public_path
#
sub get_public_path
{
  my (@pairs, $command_type, $passed_in, $item, $mark);

  if ($query eq "")
  {
    return 0;
  }

  @pairs    = split(/&/,$query);

  foreach $item(@pairs)
  {
    ($command_type,$passed_in) = split(/=/,$item,2);

    if ($command_type eq "path")
    {
      $public_path = $passed_in;
      $public_path =~ s/%2F/\//g;

      return 1;
    }
  }

  return 0;
}

