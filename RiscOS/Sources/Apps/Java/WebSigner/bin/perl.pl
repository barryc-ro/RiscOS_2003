#!/usr/local/bin/perl 

$RES_BASE = "Resources.Java.WebSigner";
$PATH_SRC = "./Resources/src";
$PATH_BASE = "./Resources/classes";

@files=`find $PATH_SRC -print`;
@files = sort @files;
$last_trunc="";
$suffix = 0;

mkdir($PATH_BASE, 0777);

foreach (@files) {
  chop;
  
  $path = $_;
  $path =~ s|$PATH_SRC(.*)|$1|;
  
#  print "path: '$path'\n";
  if ($path ne "" && $path ne "." && $path ne "..") {
    
    if (-f $_) {
      $path =~ s|(.*)/[^/]*|$1|;
      
      $path_base = $path;
      $path_base =~ s|(.*)/[^/]*|$1|;
      
      $path_leaf = $path;
      $path_leaf =~ s|.*/([^/]*)|$1|;
      $path_leaf_trunc = substr($path_leaf, 0, 10);
      
#      print "composite: $PATH_BASE$path_base/$path_leaf_trunc\n";
      
      $class = $_;
      $class =~ s|.*/([^/.]*)\.class|$1|g;
      
      $class_trunc = substr($class, 0, 8);
      
      if ($class_trunc eq $last_class_trunc) {
	$class_trunc = "$class_trunc$suffix";
	$suffix += 1;
      } else {
	$last_class_trunc = $class_trunc;
	$suffix = 0;
      }
      
      $out = `cp -r $PATH_SRC/$path_base/$path_leaf/$class.class $PATH_BASE$path_base/$path_leaf_trunc/$class_trunc`;
      
      $path_trunc_dotted = "$path_base/$path_leaf_trunc";
      $path_trunc_dotted =~ s|/|\.|g;

      $path_dotted = "$path_base/$path_leaf";
      $path_dotted =~ s|/|\.|g;
      
      #      print "$path $class $trunc\n";
      print "Resources.classes$path_trunc_dotted.$class_trunc $RES_BASE$path_dotted.$class/class\n";
    }
    elsif (-d $_) {
      
      $path_base = $path;
      $path_base =~ s|(.*)/[^/]*|$1|;
      
      $path_leaf = $path;
      $path_leaf =~ s|.*/([^/]*)|$1|;
      $path_leaf_trunc = substr($path_leaf, 0, 10);
      
#      print "dir: $PATH_BASE$path_base/$path_leaf_trunc\n";
      mkdir("$PATH_BASE/$path_base/$path_leaf_trunc", 0777);
    }
  }  
}
