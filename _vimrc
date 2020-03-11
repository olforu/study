set nocompatible
set encoding=utf-8
set termencoding=utf-8
set fileencoding=utf-8
set fileencodings=ucs-bom,utf-8,chinese,cp936
set nu
colorscheme evening
source $VIMRUNTIME/vimrc_example.vim
source $VIMRUNTIME/mswin.vim
source $VIMRUNTIME/delmenu.vim
source $VIMRUNTIME/menu.vim
language messages zh_CN.utf-8
behave mswin



"set Tab=4 spaces
set ts=4

"set ignore case
set ignorecase



"//////////vim shortcut/////////////////
let mapleader="\<space>"
nnoremap <Leader>co   :copen<CR>
nnoremap <Leader>w :w<CR>
nnoremap <Leader>q :q<CR>
nnoremap <Leader>c bye
cnoremap <C-p> <C-r>"
nnoremap <Leader>f :/
nnoremap <Leader>r :%s///g
map <F9> :tabnew<CR>
map <F2> :bn<CR>
map <S-F2> :bp<CR>
map <F6> :copen<CR>
map <S-F6> :cclose<CR>
map <F5> yiw:grep "<C-r>"": -i -n -S .\*
nnoremap <Leader>n :cn<CR>
nnoremap <Leader>p :cp<CR>
"map <Leader>y "+y<CR>
"map <Leader>p "+p<CR>

"vundle start
set nocompatible " be iMproved, required
filetype off                  " required

" 将Vundle插件的目录添加到gvim的运行时变量中
set rtp+=$HOME/.vim/bundle/Vundle.vim/
call vundle#begin('$USERPROFILE/.vim/bundle/')
" All of your Plugins must be added before the following line
Plugin 'VundleVim/Vundle.vim'
Plugin 'scrooloose/nerdtree'
Plugin 'https://github.com/vim-scripts/taglist.vim'
Plugin 'Yggdroot/LeaderF'
Plugin 'https://github.com/will133/vim-dirdiff'
Plugin 'brookhong/cscope.vim'
call vundle#end()            " required
filetype plugin indent on    " required
"vundle end

"//////// 设置NerdTree////////
map <F3> :NERDTreeMirror<CR>
map <F3> :NERDTreeToggle<CR>

"//////////Taglist setting//////////
let Tlist_Show_One_File=1     "不同时显示多个文件的tag，只显示当前文件的    
let Tlist_Exit_OnlyWindow=1   "如果taglist窗口是最后一个窗口，则退出vim   
let Tlist_Ctags_Cmd = 'C:\Ctags\ctags.exe'"将taglist与ctags关联 
let Tlist_Use_Right_Window=1
map <F8> :TlistToggle<CR>


"//////////Ctrlp setting//////////
"let g:ctrl_map = 'c-p'
"let g:ctrl_cmd = 'CtrlP'
"let g:ctrlp_working_path_mode=0
"let g:ctrlp_regexp=1
"let g:ctrlp_by_filename=1
"let g:ctrlp_mruf_max=500
"let g:ctrlp_max_files=200000
"let g:ctrlp_follow_symlinks=1
"let g:ctrlp_max_depth=10

"//////////LeaderF setting//////////
let g:Lf_CommandMap = {'<Tab>':['<Esc>']}
let g:Lf_ShortcutF = '<C-P>'

set diffexpr=MyDiff()
function MyDiff()
  let opt = '-a --binary '
  if &diffopt =~ 'icase' | let opt = opt . '-i ' | endif
  if &diffopt =~ 'iwhite' | let opt = opt . '-b ' | endif
  let arg1 = v:fname_in
  if arg1 =~ ' ' | let arg1 = '"' . arg1 . '"' | endif
  let arg2 = v:fname_new
  if arg2 =~ ' ' | let arg2 = '"' . arg2 . '"' | endif
  let arg3 = v:fname_out
  if arg3 =~ ' ' | let arg3 = '"' . arg3 . '"' | endif
  let eq = ''
  if $VIMRUNTIME =~ ' '
    if &sh =~ "\<cmd"
      let cmd = '""' . $VIMRUNTIME . '\diff"'
      let eq = '"'
    else
      let cmd = substitute($VIMRUNTIME, ' ', '" ', '') . '\diff"'
    endif
  else
    let cmd = $VIMRUNTIME . '\diff'
  endif
  silent execute '!' . cmd . ' ' . opt . arg1 . ' ' . arg2 . ' > ' . arg3 . eq
endfunction



" Go to last file(s) if invoked without arguments.
autocmd VimLeave * nested if (!isdirectory($HOME . "/.vim")) |
    \ call mkdir($HOME . "/.vim") |
    \ endif |
    \ execute "mksession! " . $HOME . "/.vim/Session.vim"

autocmd VimEnter * nested if argc() == 0 && filereadable($HOME . "/.vim/Session.vim") |
    \ execute "source " . $HOME . "/.vim/Session.vim"
