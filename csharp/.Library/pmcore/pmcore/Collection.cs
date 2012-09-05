﻿using System;
using System.Collections.Generic;
using System.Text;

namespace pmcore
{
    public class Collection
    {
        /// <summary>
        /// Структура содержащий ШАпку файла
        /// </summary>
        public struct Param
        {
            public int   FileVersion;
            public int   CanalCount;
            public float ScaleFactor;
            public float SamplingTime;
        }

        /// <summary>
        /// класс, работает с файлами для коллекции
        /// </summary>
        private class CFile
        {
            private List<string>      _vectors_name;
            private List<bool>        _vectors_visibility;
            private List<Vector> _vectors;
            private Param             _param;
            private int               _last_saved_pos;

            public CFile( List<string> names, List<bool> visible, List<Vector> vectors, Param param )
            {
                _vectors_name   = names;
                _vectors_visibility = visible;
                _vectors   = vectors;
                _param   = param;

                _last_saved_pos = 0;
            }

            public void Write_HeadTextFile( System.IO.StreamWriter sw, int head_type )
            {
                sw.WriteLine("FileVersion\t\t= "         + _param.FileVersion.ToString());
                sw.WriteLine("SamplingTime[seconds]\t= " + _param.SamplingTime.ToString());
                sw.WriteLine("ScaleFactor\t\t= "         + _param.ScaleFactor.ToString());

                // -----
                string local_str = "";
                int visible_vectors_count = 0;
                for ( int i = 0; i < _vectors.Count; i++)
                {
                    if ( _vectors_visibility[i] == true ){
                        visible_vectors_count++;
                        local_str = local_str + _vectors_name[i] + " ";
                    }
                }
                // -----
                 
                sw.WriteLine("CanalCount\t\t= "          + visible_vectors_count.ToString());

                sw.WriteLine("CanalNames\t\t" + local_str);
                sw.WriteLine("");
                sw.WriteLine("");

                _last_saved_pos = 0;
            }
            public void Write_UnsavedDataToTextFile( System.IO.StreamWriter sw)
            {
                // = 0;
                // -----
                string str_to_write;
                double temp_double_val;

                int items_count  = _vectors.Count;
                int values_count = _vectors[0].data.Count;
                for ( int k = _last_saved_pos; k < values_count; k++)
                {
                    temp_double_val =  _vectors[0].data[k];
                    str_to_write    = "";
                    bool is_value_in_line = true;

                    for ( int i = 0; i < items_count; i++)
                    {
                        if ( _vectors_visibility[i] == true )
                        {
                            temp_double_val = _vectors[i].data[k];

                            if ( is_value_in_line == true )
                            {
                                str_to_write = temp_double_val.ToString("f9");     
                                is_value_in_line = false;
                            }
                            else
                                str_to_write += "\t" + temp_double_val.ToString("f9");
                        }


                    }
                    sw.WriteLine(str_to_write);
                }
                _last_saved_pos = values_count;
                sw.Flush();
            }

            public void Load_TextFile( string FileName, object ProgressBar )
            {
                System.IO.StreamReader sr = new System.IO.StreamReader(FileName);


                while ( sr.EndOfStream == false ) 
                {
                    // ---- Чтение строки с файла
                    string []SplitedStringArray = sr.ReadLine().Split('\t','=',' ');
                    int ssa_length = SplitedStringArray.Length;

                    #region Декодирование Шапки файла
                        if (SplitedStringArray[0] == "FileVersion")
                            for (int i = 1; i < ssa_length; i++)
                                if ( Int32.TryParse(SplitedStringArray[i], out _param.FileVersion) == true) break;

                        if (SplitedStringArray[0] == "SamplingTime[seconds]") 
                            for (int i = 1; i < ssa_length; i++)
                                if (Single.TryParse(SplitedStringArray[i], out _param.SamplingTime) == true) break;

                        if (SplitedStringArray[0] == "ScaleFactor") 
                            for (int i = 1; i < ssa_length; i++)
                                if (Single.TryParse(SplitedStringArray[i], out _param.ScaleFactor) == true) break;

                        if (SplitedStringArray[0] == "CanalCount") 
                            for (int i = 1; i < ssa_length; i++)
                                if (Int32.TryParse(SplitedStringArray[i], out _param.CanalCount) == true) break;

                        if (SplitedStringArray[0] == "CanalNames") 
                        {
                            _vectors_name.Clear();
                            _vectors_visibility.Clear();
                            for (int i = 1; i < ssa_length; i++)
                            {
                                if (SplitedStringArray[i] != "")
                                {
                                    _vectors_name.Add(SplitedStringArray[i]);
                                    _vectors_visibility.Add(true);
                                }
                            }
                        }
                    #endregion

                    // Проверка начала данных ( проверка, могу ли я преобразовать данные во флоат)
                    float temporary_double;
                    if ( Single.TryParse(SplitedStringArray[0], out temporary_double) == false ) continue;
                
                    // Создаем новые Лиисты
                    _vectors.Clear();
                    int ListsCount = SplitedStringArray.Length;
                    for (int i = 0; i < ListsCount; i++)
                        _vectors.Add(new Vector());

                    // Запоняю первую строчку "Листов"
                    double[] fArray = Array.ConvertAll<string, double>(SplitedStringArray, double.Parse);
                    for (int i = 0; i < ListsCount; i++)
                        _vectors[i].Add(fArray[i]);

                    //Console.Beep();

                    // Чтение остальных данных, пока не конец файла
                    while (sr.EndOfStream == false)
                    {
                        // Чтение строки с файла
                        SplitedStringArray = sr.ReadLine().Split('\t');
                        fArray = Array.ConvertAll<string, double>(SplitedStringArray, double.Parse);

                        for (int i = 0; i < ListsCount; i++)
                            _vectors[i].Add(fArray[i]);
                    }
                }

                //Console.WriteLine("loaded");
                sr.Close();
            }

            public void Save_TextFile( string FileName, object ProgressBar )
            {
                System.IO.StreamWriter sw = new System.IO.StreamWriter(FileName);

                Write_HeadTextFile(sw, 1);
                Write_UnsavedDataToTextFile( sw );

                //Console.WriteLine("saved");
                //Console.ReadLine();

                sw.Close();
            }

            public void Load_Binary(string FileName)
            {
                System.IO.FileStream fs = new System.IO.FileStream(FileName, System.IO.FileMode.Open);
                fs.Close();

                throw new Exception(" todo ");
            }

            public void Save_Binary(string FileName)
            {
                System.IO.FileStream fs = new System.IO.FileStream(FileName, System.IO.FileMode.CreateNew);
                fs.Close();

                throw new Exception(" todo ");
            }
        }

        private class Vector
        {
            public double Maximum;
            public double Minimum;
            public List<double> data;

            public Vector()
            {
                Maximum = double.MinValue;
                Minimum = double.MaxValue;

                data = new List<double>();
            }

            public Vector( Vector inVector )
            {
                Maximum = inVector.Maximum;
                Minimum = inVector.Minimum;

                data = new List<double>(inVector.data);
            }

            public void Add( double Value)
            {
                data.Add(Value);
                if ( Minimum > Value ) Minimum = Value;
                if ( Maximum < Value ) Maximum = Value;
            }

            public void Clear()
            {
                data.Clear();

                Maximum = double.MinValue;
                Minimum = double.MaxValue;
            }

        }


        private List<string>      _vectors_name;
        private List<bool>        _vectors_visibility;
        private List<Vector>      _vectors;
        /// <summary>
        /// Файловая часть работы с колекцией,
        /// </summary>
        private CFile              File;
        public Param FileParam;

        public Collection(int ItemsCount = 1)
        {
            _vectors_name   = new List<string>();
            _vectors_visibility = new List<bool>();
            _vectors = new List<Vector>();

            File    = new CFile(_vectors_name, _vectors_visibility, _vectors, FileParam);

            FileParam.CanalCount   = ItemsCount;
            FileParam.FileVersion  = 1;
            FileParam.SamplingTime = 1.0f;
            FileParam.ScaleFactor  = 1.0f;

            for (int i = 0; i < ItemsCount; i++)
            {
                _vectors_name.Add("Canal_" + i.ToString());
                _vectors_visibility.Add(true);
            }
            set_Vectors_Count(ItemsCount);
        }

        public Collection( Collection inCollection )
        {
            _vectors_name       = new List<string>(inCollection._vectors_name);
            _vectors_visibility = new List<bool>(inCollection._vectors_visibility);
            _vectors            = new List<Vector>();

            File    = new CFile(_vectors_name, _vectors_visibility, _vectors, FileParam);

            FileParam.CanalCount   = inCollection.FileParam.CanalCount;
            FileParam.FileVersion  = inCollection.FileParam.FileVersion;
            FileParam.SamplingTime = inCollection.FileParam.SamplingTime;
            FileParam.ScaleFactor  = inCollection.FileParam.ScaleFactor;

            int ItemsCount = inCollection._vectors.Count;

            // Копирование содержимого 
            _vectors.Clear();
            for (int i = 0; i < ItemsCount; i++)
                _vectors.Add(new Vector(inCollection._vectors[i]));

            GC.Collect();
        }

        public Collection(  string FileName, object ProgressBar )
        {
            _vectors_name       = new List<string>();
            _vectors_visibility = new List<bool>();
            _vectors            = new List<Vector>();

            File    = new CFile(_vectors_name, _vectors_visibility, _vectors, FileParam);

            File.Load_TextFile( FileName, ProgressBar );
        }
        
        #region методы Collection 
        public void set_Vectors_Count(int vectors_count)
        {
            for (int i = 0; i < _vectors.Count; i++ )
                _vectors[i].Clear();
            _vectors.Clear();

            GC.Collect();

            for (int i = 0; i < vectors_count; i++)
                _vectors.Add(new Vector());
        }

        public void set_VectorName( int index, string name)
        {
            _vectors_name[index] = name;
        }
        public string get_VectorName( int index )
        {
            return _vectors_name[index];
        }

        public void set_VectorVisibility( int index, bool visibility)
        {
            _vectors_visibility[index] = visibility;
        }
        public bool get_VectorVisibility( int index )
        {
            return _vectors_visibility[index];
        }

        public List<double> this[int index]
        {
            get { return _vectors[index].data; }
        }

        /// <summary>
        /// Добавляет значение в колекцию в первый Vector.
        /// Метод справделив для размерности колекции = 1 (VectorsCount = 1).
        /// Дополнительных проверок на размерность нету, так что, акуратно  :)
        /// </summary>
        /// <param name="Value">Добавляемое значение</param>
        public void Add(double Value)
        {
            _vectors[0]. Add(Value);
        }

        /// <summary>
        /// Добавляет значение в колекцию в Vector с номером index.
        /// Дополнительных проверок на размерность нету, так что, акуратно  :)
        /// </summary>
        /// <param name="Value">Добавляемое значение</param>
        public void Add(int index, double Value)
        {
            _vectors[index].Add(Value);
        }

        /// <summary>
        /// Ощищает масивы данных, в созданой колекции
        /// </summary>
        public void Clear()
        {
            for ( int i = 0 ; i < _vectors.Count; i++)
                _vectors[i].Clear();

            GC.Collect();
        }

        /// <summary>
        /// Метод опредиляющий количество "видимых" каналов
        /// </summary>
        /// <returns>Количество "видимых" каналов</returns>
        public int get_CountVisibleVectors()
        {
            int CountVisible = 0;

            foreach (bool visible in _vectors_visibility)
                if (visible == true) CountVisible++;

            return CountVisible;
        }

        public int VectorsCount
        {
            get {  return _vectors.Count; }
        }

        public void Load_TextFile( string FileName, object ProgressBar )
        {
            File.Load_TextFile(FileName, ProgressBar);
        }

        public void Save_TextFile( string FileName, object ProgressBar )
        {
            File.Save_TextFile(FileName, ProgressBar);
        }
        
        /// <summary>
        /// Метод возрачает коллекцию, с заданими индексами векторов
        /// </summary>
        /// <param name="vector_indexes">масив индексов веторов</param>
        /// <returns>Новая коллекция</returns>
        public Collection get_SubCollection( int[] vector_indexes)
        {
            Collection result = new Collection(vector_indexes.Length);
 
            result._vectors.Clear();
            for ( int i = 0; i < vector_indexes.Length; i ++)
               result._vectors.Add( new Vector(this._vectors[vector_indexes[i]]));

            return result;
        }

        #endregion
    }
}