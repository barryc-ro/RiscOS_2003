#!/usr/bin/perl
#
# m_submit.pl - Called from create.pl and modify.pl.  Takes the user's input and after validation
#               writes the new Users file.
#

print "Content-type: text/html\r\n\r\n";
use strict;

# global variables
#
my ($query)   = $ENV{'QUERY_STRING'};
my ($header_html) = "Complete_H";

# **** The arrays of records
my (@usernames, @passwords, @addresses, @flags, @localpophostnames, @localsmtphostnames);
my ($global_pop_hostname, $global_smtp_hostname, $poll_delay, $poll_for_mail);
my ($edithosts, $blocked) = 0;

# **** The path to the baseline public directory, the path to the User file, command type
my ($public_path, $accounts_file, $command_type);

# global pointer to current record, total number of records
my ($record_number, $number_of_records);

# **** The account number that is being modified or created
my ($user);


# ********************************************************


# main
#

if (get_public_path())
{
  $accounts_file = $public_path."/NCMail/Users";

  read_accounts_file($accounts_file);

  if (get_user_number_and_command_type())
  {
    if ($command_type eq "delete")
    {
      # Make sure there is always a default record
      if ($flags[$user] == 1)
      {
        if ($user == 0)
        {
          $flags[1] = 1;
        }
        else
        {
          $flags[0] = 1;
        }
      }

      # Check there is more than one record in the file?  Don't allow last one to be deleted?

      write_accounts_file($accounts_file, 0);

      output_header_html($header_html);
      output_buttons();
      output_main_html();
    }

    if ($command_type eq "modify")
    {
      $record_number = $user;

      change_array_details();
      write_accounts_file($accounts_file, 1);

      output_header_html($header_html);
      output_buttons();
      output_main_html();
    }

    if ($command_type eq "create")
    {
      # **** Point to the next free account number
      $record_number = $number_of_records;
      $number_of_records += 1;

      change_array_details();
      write_accounts_file($accounts_file, 1);

      output_header_html($header_html);
      output_buttons();
      output_main_html();
    }

    if ($command_type eq "block")
    {
      $record_number = $user;

      write_accounts_file($accounts_file, 2);

      output_header_html($header_html);
      output_buttons();
      output_main_html();
    }

    if ($command_type eq "unblock")
    {
      $record_number = $user;

      write_accounts_file($accounts_file, 3);

      output_header_html($header_html);
      output_buttons();
      output_main_html();
    }
  }
}
else
{
  output_header_html($header_html);
  output_buttons(1);
}


# read the accounts file
#
sub read_accounts_file
{
  my ($file) = @_;
  my ($line, @fields, $temp, $i);

  if (!open(FILE,$file))
  {
      print("Error opening $file : $!\n");
      return;
  }

  $number_of_records = 0;

  # **** Search through the users file for the records
  while ($line = <FILE>)
  {
    @fields = split(/ /,$line);

    if ($fields[0] eq "POP3hostname" or $fields[0] eq "|POP3hostname")
    {
      $global_pop_hostname = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "SMTPhostname" or $fields[0] eq "|SMTPhostname")
    {
      $global_smtp_hostname = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "PollDelay" or $fields[0] eq "|PollDelay")
    {
      $poll_delay = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "PollForMail" or $fields[0] eq "|PollForMail")
    {
      $poll_for_mail = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "EditHosts" or $fields[0] eq "|EditHosts")
    {
      $edithosts = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "Blocked" or $fields[0] eq "|Blocked")
    {
      $blocked = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "pop3username" or $fields[0] eq "|pop3username")
    {
      $usernames[$number_of_records] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "pop3password" or $fields[0] eq "|pop3password")
    {
      $passwords[$number_of_records] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "flags" or $fields[0] eq "|flags")
    {
      $flags[$number_of_records] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "localpop3" or $fields[0] eq "|localpop3")
    {
      $localpophostnames[$number_of_records] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "localsmtp" or $fields[0] eq "|localsmtp")
    {
      $localsmtphostnames[$number_of_records] = substr($fields[1], 0, length($fields[1]) - 1);
    }

    if ($fields[0] eq "emailaddress" or $fields[0] eq "|emailaddress")
    {
      ($addresses[$number_of_records],$temp) = split("@", substr($fields[1], 0, length($fields[1]) - 1));

      # increment the number_of_records when the email address is found as it is the last part of each account record
      $number_of_records += 1;
    }

  }

  close (FILE);

  # Check that the fields are all filled, even if just the default settings
  for ($i = 0; $i < $number_of_records; $i++)
  {
    if ($localpophostnames[$i] eq "" or $localpophostnames[$i] eq "-") { $localpophostnames[$i] = $global_pop_hostname; }
    if ($localsmtphostnames[$i] eq "" or $localsmtphostnames[$i] eq "-") { $localsmtphostnames[$i] = $global_smtp_hostname; }
  }
}


# write the new accounts file
#
sub write_accounts_file
{
  my ($file, $method) = @_;
  my ($line, $j);

  if (!open(FILE, ">$file"))
  {
      print("Error opening $file : $!\n");
      return;
  }

  # Write the contents here

  print FILE "POP3hostname ".$global_pop_hostname."\n";
  print FILE "SMTPhostname ".$global_smtp_hostname."\n";
  print FILE "PollDelay ".$poll_delay."\n";
  print FILE "PollForMail ".$poll_for_mail."\n";
  print FILE "EditHosts ".$edithosts."\n";
  print FILE "Blocked ".$blocked."\n\n";

  for ($j = 0; $j < $number_of_records; $j++)
  {
    if ($method == 0 and $j eq $user)
    {
      # Don't write the record if deleting!
    }
    else
    {
      if ($method == 2) # BLOCKING THE WHOLE FILE
      {
        print FILE "|pop3username ".$usernames[$j]."\n";
        print FILE "|pop3password ".$passwords[$j]."\n";
        print FILE "|flags ".$flags[$j]."\n";

        print FILE "|localpop3 ".$localpophostnames[$j]."\n";
        print FILE "|localsmtp ".$localsmtphostnames[$j]."\n";

        print FILE "|emailaddress ".$addresses[$j]."@".$localpophostnames[$j]."\n\n";
      }
      else
      {
        print FILE "pop3username ".$usernames[$j]."\n";
        print FILE "pop3password ".$passwords[$j]."\n";
        print FILE "flags ".$flags[$j]."\n";

        print FILE "localpop3 ".$localpophostnames[$j]."\n";
        print FILE "localsmtp ".$localsmtphostnames[$j]."\n";

        print FILE "emailaddress ".$addresses[$j]."@".$localpophostnames[$j]."\n\n";
      }
    }
  }

  truncate (FILE, tell (FILE)) or die "Could not truncate: $!\n";

  close (FILE);
}


# modify the arrays with the new details
#
# pairs         = the query string split up into <command>=<value> parts
# record_number = either the first free account number (when creating), or the account to alter (when modifying)
#
sub change_array_details
{
  my (@pairs, $name, $content, $item, $k);

  @pairs    = split(/&/,$query);

  foreach $item(@pairs)
  {
    ($name,$content) = split(/=/,$item,2);

    if ($name eq "address")
    {
      if ($content ne "")
      {
        $addresses[$record_number] = $content;
      }
    }

    if ($name eq "username")
    {
      if ($content ne "")
      {
        $usernames[$record_number] = $content;
      }
    }

    if ($name eq "flags")
    {
      $flags[$record_number] = 0;

      if ($content ne "")
      {
        # **** Clear all the other records' flags if the modified or new one has the flag set
        if ($content eq "1" or $content eq "true")
        {
          for ($k = 0; $k < $number_of_records; $k++)
          {
            $flags[$k] = 0;
          }

          $flags[$record_number] = 1;
        }
      }
    }

    if ($name eq "pophostname")
    {
      if ($content ne "")
      {
        $localpophostnames[$record_number] = $content;
      }
    }

    if ($name eq "smtphostname")
    {
      if ($content ne "")
      {
        $localsmtphostnames[$record_number] = $content;
      }
    }

    if ($name eq "old_password")
    {
      if ($content ne "")
      {
        $passwords[$record_number] = $content;
      }
    }

    if ($name eq "new_password")
    {
      if ($content ne "")
      {
        # $new_password = $content;
        $passwords[$record_number] = $content;
      }
    }

#    if ($name eq "retype_password")
#    {
#      if ($content ne "")
#      {
#        $retype_password = $content;
#      }
#    }
  }
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
  # **** Output the error message
  print "<BR><BR><BR><BR><BR><BR>\n<CENTER>\n\n";

  if ($command_type eq "delete")
  {
    print "<FONT FACE=ncoffline SIZE=êêglobal_fontsize_xlgëë>êêm_submit_deletesuccessëë</FONT>\n";
  }

  if ($command_type eq "modify")
  {
    print "<FONT FACE=ncoffline SIZE=êêglobal_fontsize_xlgëë>êêm_submit_modifysuccessëë</FONT>\n";
  }

  if ($command_type eq "create")
  {
    print "<FONT FACE=ncoffline SIZE=êêglobal_fontsize_xlgëë>êêm_submit_createsuccessëë</FONT>\n";
  }

  if ($command_type eq "block")
  {
    print "<FONT FACE=ncoffline SIZE=êêglobal_fontsize_xlgëë>êêm_submit_blocksuccessëë</FONT>\n";
  }

  if ($command_type eq "unblock")
  {
    print "<FONT FACE=ncoffline SIZE=êêglobal_fontsize_xlgëë>êêm_submit_unblocksuccessëë</FONT>\n";
  }
}


# output_buttons
#
sub output_buttons
{
  my ($n) = @_;

  print "<TABLE WIDTH=\"100%\" ALIGN=\"CENTER\" BORDER=0 CELLSPACING=0>";
  print "  <TR>";
  print "    <TD WIDTH=\"100%\" ALIGN=CENTER>";
  print "      <FONT FACE=ncoffline SIZE=êêglobal_fontsize_lgëë>";
  print "      <FORM METHOD=\"GET\" ACTION=\"accounts.pl\" Target=\"_top\">";
  print "        <INPUT TYPE=HIDDEN NAME=\"path\" VALUE=\"".$public_path."\">";
  print "        <INPUT TYPE=SUBMIT VALUE=\"êêm_submit_b_accountsëë\" BORDERIMAGE=\"icontype:buttmid_bord\" SELIMAGE=\"icontype:buttmid_sel\" WIDTH=110 HEIGHT=40>";
  print "      </FORM>";
  print "    </TD>";
  print "  </TR>";
  print "</TABLE>";

}

# get_user_number_and_command_type
#
# Reads the user number that should be modified from the query string...?user=
#
sub get_user_number_and_command_type
{
  my (@pairs, $passed_in, $item, $action_type, $return_value);

  if ($query eq "")
  {
    return 0;
  }

  @pairs    = split(/&/,$query);
  $return_value = 0;

  foreach $item(@pairs)
  {
    ($action_type,$passed_in) = split(/=/,$item,2);

    if ($action_type eq "delete" or $action_type eq "modify" or $action_type eq "create" or $action_type eq "block" or $action_type eq "unblock")
    {
      $command_type = $action_type;
    }

    if ($action_type eq "user")
    {
      $user = $passed_in;

      $return_value = 1;
    }
  }

  # create doesn't pass in a user number so return true no matter what
  if ($command_type eq "create")
  {
    $return_value = 1;
  }

  return ($return_value);
}


# get_public_path
#
sub get_public_path
{
  my (@pairs, $passed_in, $item, $action_type);

  if ($query eq "")
  {
    return 0;
  }

  @pairs    = split(/&/,$query);

  foreach $item(@pairs)
  {
    ($action_type,$passed_in) = split(/=/,$item,2);

    if ($action_type eq "path")
    {
      $public_path = $passed_in;
      $public_path =~ s/%2F/\//g;

      return 1;
    }
  }

  return 0;
}
