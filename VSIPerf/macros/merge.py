import os
import sys

#output_dir='/nfs/slac/atlas/fs1/d/mgignac/VSI/v02/'
#merge_dir='/nfs/slac/atlas/fs1/d/mgignac/VSI_merged/'

#output_dir='/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0/HNL_samples/'
output_dir='/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0/N1_samples/'
#merge_dir='/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_m_31319/'
merge_dir='/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_m/'

#output_dir='/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/20180228_TrkCleaning'
#merge_dir='/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_cleaned_31319'

if not os.path.exists(merge_dir):
  os.makedirs(merge_dir)

# Loop over all files, merge into a common output file
for fn in os.listdir(output_dir):
  output_file_name = '%s/merged_%s' % (merge_dir,fn)
  eff_output_name  = output_file_name.replace('merged','processed')
  list_of_samples = ''
  for s in os.listdir(output_dir+'/'+fn):
    list_of_samples += output_dir+'/'+fn+'/'+s+' '

  # Now Hadd them
  cmd = 'hadd -f %s %s' % (output_file_name,list_of_samples);
  os.system(cmd)

  # After Hadd, run them through our DVEffAnalysis script
  ana_cmd = 'root -l -b -q \'DVeffAnalysis.C+(\"%s\",\"%s\")\'' % (output_file_name,eff_output_name)
  os.system(ana_cmd)
